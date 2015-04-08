/*
 * libcsync -- a library to sync a directory with another
 *
 * Copyright (c) 2008-2013 by Andreas Schneider <asn@cryptomilk.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "torture.h"

#include "csync_private.h"
#include "vio/csync_vio.h"

#define CSYNC_TEST_DIR "/tmp/csync_test/"
#define CSYNC_TEST_DIRS "/tmp/csync_test/this/is/a/mkdirs/test"
#define CSYNC_TEST_FILE "/tmp/csync_test/file.txt"

#define MKDIR_MASK (S_IRWXU |S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH)

#define WD_BUFFER_SIZE 255

static char wd_buffer[WD_BUFFER_SIZE];

static void setup(void **state)
{
    CSYNC *csync;
    int rc;

    assert_non_null(getcwd(wd_buffer, WD_BUFFER_SIZE));

    rc = system("rm -rf /tmp/csync_test");
    assert_int_equal(rc, 0);

    rc = csync_create(&csync, "/tmp/csync1", "/tmp/csync2");
    assert_int_equal(rc, 0);

    csync->replica = LOCAL_REPLICA;

    *state = csync;
}

static void setup_dir(void **state) {
    int rc;
    mbchar_t *dir = c_utf8_to_locale(CSYNC_TEST_DIR);

    setup(state);

    rc = _tmkdir(dir, MKDIR_MASK);
    c_free_locale_string(dir);
    assert_int_equal(rc, 0);

    assert_non_null(getcwd(wd_buffer, WD_BUFFER_SIZE));

    rc = chdir(CSYNC_TEST_DIR);
    assert_int_equal(rc, 0);
}

static void setup_file(void **state) {
    int rc;

    setup_dir(state);

    rc = system("echo \"This is a test\" > /tmp/csync_test/file.txt");
    assert_int_equal(rc, 0);
}

static void teardown(void **state) {
    CSYNC *csync = *state;
    int rc;

    rc = csync_destroy(csync);
    assert_int_equal(rc, 0);

    rc = chdir(wd_buffer);
    assert_int_equal(rc, 0);

    rc = system("rm -rf /tmp/csync_test/");
    assert_int_equal(rc, 0);

    *state = NULL;
}

static void check_csync_vio_load(void **state)
{
    CSYNC *csync = *state;
    int rc;

    rc = csync_vio_init(csync, "owncloud", NULL);
    assert_int_equal(rc, 0);

    csync_vio_shutdown(csync);
}

static void check_csync_vio_load_wrong_proto(void **state)
{
    CSYNC *csync = *state;
    int rc;

    rc = csync_vio_init(csync, "wrong", NULL);
    assert_int_equal(rc, -1);

    csync_vio_shutdown(csync);
}

static void check_csync_vio_load_bad_plugin(void **state)
{
    CSYNC *csync = *state;
    int rc;

    rc = csync_vio_init(csync, "bad", NULL);
    assert_int_equal(rc, -1);

    csync_vio_shutdown(csync);
}

/*
 * Test directory function
 */

static void check_csync_vio_opendir(void **state)
{
    CSYNC *csync = *state;
    csync_vio_handle_t *dh;
    int rc;

    dh = csync_vio_opendir(csync, CSYNC_TEST_DIR);
    assert_non_null(dh);

    rc = csync_vio_closedir(csync, dh);
    assert_int_equal(rc, 0);
}

static void check_csync_vio_opendir_perm(void **state)
{
    CSYNC *csync = *state;
    csync_vio_handle_t *dh;
    int rc;
    mbchar_t *dir = c_utf8_to_locale(CSYNC_TEST_DIR);

    assert_non_null(dir);

    rc = _tmkdir(dir, (S_IWUSR|S_IXUSR));
    assert_int_equal(rc, 0);

    dh = csync_vio_opendir(csync, CSYNC_TEST_DIR);
    assert_null(dh);
    assert_int_equal(errno, EACCES);

    _tchmod(dir, MKDIR_MASK);
    c_free_locale_string(dir);
}

static void check_csync_vio_closedir_null(void **state)
{
    CSYNC *csync = *state;
    int rc;

    rc = csync_vio_closedir(csync, NULL);
    assert_int_equal(rc, -1);
}

static void check_csync_vio_readdir(void **state)
{
    CSYNC *csync = *state;
    csync_vio_handle_t *dh;
    csync_vio_file_stat_t *dirent;
    int rc;

    dh = csync_vio_opendir(csync, CSYNC_TEST_DIR);
    assert_non_null(dh);

    dirent = csync_vio_readdir(csync, dh);
    assert_non_null(dirent);

    csync_vio_file_stat_destroy(dirent);
    rc = csync_vio_closedir(csync, dh);
    assert_int_equal(rc, 0);
}


/*
 * Test for general functions (stat, chmod, chown, ...)
 */

static void check_csync_vio_stat_dir(void **state)
{
    CSYNC *csync = *state;
    csync_vio_file_stat_t *fs;
    int rc;

    fs = csync_vio_file_stat_new();
    assert_non_null(fs);

    rc = csync_vio_stat(csync, CSYNC_TEST_DIR, fs);
    assert_int_equal(rc, 0);

    assert_string_equal(fs->name, "csync_test");
    assert_int_equal(fs->type, CSYNC_VIO_FILE_TYPE_DIRECTORY);

    csync_vio_file_stat_destroy(fs);
}

static void check_csync_vio_stat_file(void **state)
{
    CSYNC *csync = *state;
    csync_vio_file_stat_t *fs;
    int rc;

    fs = csync_vio_file_stat_new();
    assert_non_null(fs);

    rc = csync_vio_stat(csync, CSYNC_TEST_FILE, fs);
    assert_int_equal(rc, 0);

    assert_string_equal(fs->name, "file.txt");
    assert_int_equal(fs->type, CSYNC_VIO_FILE_TYPE_REGULAR);

    csync_vio_file_stat_destroy(fs);
}

int torture_run_tests(void)
{
    const UnitTest tests[] = {
        unit_test_setup_teardown(check_csync_vio_opendir, setup_dir, teardown),
        unit_test_setup_teardown(check_csync_vio_opendir_perm, setup, teardown),
        unit_test(check_csync_vio_closedir_null),
        unit_test_setup_teardown(check_csync_vio_readdir, setup_dir, teardown),

        unit_test_setup_teardown(check_csync_vio_stat_dir, setup_dir, teardown),
        unit_test_setup_teardown(check_csync_vio_stat_file, setup_file, teardown),
    };

    return run_tests(tests);
}

