%if %{undefined suse_version}
%define cmake_custom mkdir -p build && cd build && %cmake ..
%else
%define cmake_custom %cmake
%endif

Summary: Lightweight utility to synchronize files
Name: ocsync
Version: 0.0.0
Release: 1
License: LGPL2.1
Group: System Environment/Libraries
URL: https://owncloud.org/
Source0: %{name}-%{version}.tar.bz2
BuildRequires: cmake >= 2.8
BuildRequires: pkgconfig(cmocka)
BuildRequires: pkgconfig(sqlite3)
Provides:      csync > 0.5.0
Obsoletes:     csync <= 0.5.0

%description
csync is a lightweight utility to synchronize files between two directories on
a system or between multiple systems.

It synchronizes bidirectionally and allows the user to keep two copies of files
and directories in sync. csync uses widely adopted protocols, such as smb or
sftp, so that there is no need for a server component. It is a user-level
program which means you don’t need to be a superuser or administrator.

%package devel
Summary: CSync development files
Group: Development/Libraries
Requires: %{name} = %{version}-%{release}
%description devel
%{summary}

%prep
%setup -q -n %{name}-%{version}/csync

%build
%cmake_custom
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%{!?cmake_install: %global cmake_install cd build && make install DESTDIR=%{buildroot}}
%cmake_install

%files
%defattr(-,root,root,-)
%{_libdir}/*.so*

%files devel
%defattr(-,root,root,-)
%dir %{_includedir}/%{name}
%{_includedir}/%{name}/*.h

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig
