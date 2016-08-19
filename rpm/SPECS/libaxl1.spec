%define release_date %(date +"%a %b %d %Y")
%define axl_version %(cat VERSION)

Name:           axl
Version:        %{axl_version}
Release:        5%{?dist}
Summary:        LibAxl Fast and efficient XML 1.0 implementation
Group:          System Environment/Libraries
License:        LGPLv2+ 
URL:            http://www.aspl.es/xml
Source:         %{name}-%{version}.tar.gz
# %_topdir 	/usr/src/libaxl/rpm
# BuildRequires:  libidn-devel
# BuildRequires:  krb5-devel
# BuildRequires:  libntlm-devel
# BuildRequires:  pkgconfig

%define debug_package %{nil}

%description
Libaxl is a XML 1.0 implementation written in C which aims
to be fast, efficient and small. This is the development
package.

%prep
%setup -q

%build
%configure --prefix=/usr --sysconfdir=/etc
make clean
make %{?_smp_mflags}

%install
make install DESTDIR=%{buildroot} INSTALL='install -p'
find %{buildroot} -name '*.la' -exec rm -f {} ';'
# %find_lang %{name}

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

# %files -f %{name}.lang
%doc AUTHORS COPYING NEWS README THANKS
# %{_libdir}/libaxl.so.*

# %files devel
# %doc COPYING
# %{_includedir}/axl*
# %{_libdir}/libaxl.so
# %{_libdir}/pkgconfig/axl.pc

# axl-knife package
%package -n axl-knife
Summary: Command line tool built on top of Axl Library
Group: System Environment/Libraries
Requires: libaxl1
Requires: libaxlns1
%description  -n axl-knife
Command line tool built on top of Axl Library that allows
to implement varios operations from the command line.
%files -n axl-knife
/usr/bin/axl-knife

# python-axl package
%package -n python-axl
Summary: Python bindings for Axl Library
Group: System Environment/Libraries
Requires: libaxl1
%description  -n python-axl
Python bindings for Axl Library
%files -n python-axl
%if 0%{?el6}
/usr/lib/python2.6/site-packages/axl/*
%endif
%if 0%{?el7}
/usr/lib/python2.7/site-packages/axl/*
%endif

# python-axl-dev package
%package -n python-axl-dev
Summary: Development headers for Axl Python bindings
Group: System Environment/Libraries
Requires: libaxl-dev
%description  -n python-axl-dev
Python Axl bindings development headers.
%files -n python-axl-dev
/usr/include/py_axl/*

# libaxlbabel-dev package
%package -n libaxlbabel-dev
Summary: Extended encodings support for for LibAxl. Development package.
Group: System Environment/Libraries
Requires: libaxlbabel1
Requires: libaxl-dev
%description  -n libaxlbabel-dev
Extended encodings support for for LibAxl. Development package.
%files -n libaxlbabel-dev
/usr/include/axl/axl_babel*
/usr/lib64/pkgconfig/axl-babel.pc

# libaxlbabel-dev package
%package -n libaxlns-dev
Summary: Namespace support for LibAxl
Group: System Environment/Libraries
Requires: libaxl-dev
Requires: libaxlns1
%description  -n libaxlns-dev
Extended encodings support for for LibAxl. Development package.
%files -n libaxlns-dev
/usr/include/axl/axl_ns*
/usr/lib64/pkgconfig/axl-ns.pc

# libaxl-dev package
%package -n libaxl-dev
Summary: Fast and efficient XML 1.0 implementation (devel files)
Group: System Environment/Libraries
Requires: libaxl1
%description  -n libaxl-dev
Fast and efficient XML 1.0 implementation (devel files)
%files -n libaxl-dev
/usr/include/axl/axl.h
/usr/include/axl/axl_config.h
/usr/include/axl/axl_decl.h
/usr/include/axl/axl_doc.h
/usr/include/axl/axl_dtd.h
/usr/include/axl/axl_error.h
/usr/include/axl/axl_factory.h
/usr/include/axl/axl_hash.h
/usr/include/axl/axl_list.h
/usr/include/axl/axl_log.h
/usr/include/axl/axl_node.h
/usr/include/axl/axl_ns.h
/usr/include/axl/axl_ns_doc.h
/usr/include/axl/axl_ns_node.h
/usr/include/axl/axl_stack.h
/usr/include/axl/axl_stream.h
/usr/lib64/pkgconfig/axl.pc

# libaxl1 package
%package -n libaxl1
Summary: Fast and efficient XML 1.0 implementation
Group: System Environment/Libraries
Requires: glibc
%description  -n libaxl1
Fast and efficient XML 1.0 implementation
%files -n libaxl1
/usr/lib64/libaxl.*

# libaxlbabel1 package
%package -n libaxlbabel1
Summary: Fast and efficient XML 1.0 implementation (extended encoding support)
Group: System Environment/Libraries
Requires: libaxl1
%description  -n libaxlbabel1
Fast and efficient XML 1.0 implementation (extended encoding support)
%files -n libaxlbabel1
/usr/lib64/libaxl-babel.*

# libaxlns1 package
%package -n libaxlns1
Summary: Fast and efficient XML 1.0 implementation (extended encoding support)
Group: System Environment/Libraries
Requires: libaxl1
%description  -n libaxlns1
Fast and efficient XML 1.0 implementation (extended encoding support)
%files -n libaxlns1
/usr/lib64/libaxl-ns.*


%changelog
%include rpm/SPECS/changelog.inc

