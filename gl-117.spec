%define name gl-117
%define version 0.8

Summary: An Action Flight Simulation
Name: %{name}
Version: %{version}
Release: 1
Copyright: GPL
Vendor: Thomas A. Drexl <tom.drexl@gmx.de>
Url: http://home.t-online.de/home/Primetime./
Packager: Dominik Seichter <domseichter@web.de>
Group: x11/games
Source: %{name}-%{version}.tar.gz
BuildRoot: /var/tmp/%{name}-%{version}

%description
GL-117 is an action flight simulator for Linux/Unix and MSWindows. Enter the Eagle Squadron and succeed in several challanging missions leading though different landscapes. Five predefined levels of video quality and an amount of viewing ranges let you perfectly adjust the game to the performance of your system. Joystick, mouse, sound effects, music... 

%prep
%setup
./configure

%build

# Setup for parallel builds
numprocs=`egrep -c ^cpu[0-9]+ /proc/stat || :`
if [ "$numprocs" = "0" ]; then
  numprocs=1
fi

make -j$numprocs

%install
make install-strip DESTDIR=$RPM_BUILD_ROOT

cd $RPM_BUILD_ROOT
find . -type d | sed '1,2d;s,^\.,\%attr(-\,root\,root) \%dir ,' > $RPM_BUILD_DIR/file.list.gl-117
find . -type f | sed 's,^\.,\%attr(-\,root\,root) ,' >>  $RPM_BUILD_DIR/file.list.gl-117
find . -type l | sed 's,^\.,\%attr(-\,root\,root) ,' >>  $RPM_BUILD_DIR/file.list.gl-117

%clean
rm -rf $RPM_BUILD_ROOT/*
rm -rf $RPM_BUILD_DIR/gl-117
rm -rf ../file.list.gl-117

%files -f ../file.list.gl-117





