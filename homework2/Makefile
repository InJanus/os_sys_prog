# this makefile isn’t really a standalone makefile script.  Rather, it calls
# and depends on the make scripts that are already in the source tree where
# other kernel modules would reside for the version of the kenrel you’re 
# building the module against.  This is actually a good thing, as you’ll get 
# the correct build settings and other properties for the kernel you’re using.
#
# This first line “adds” your module to the list of the modules the whole
# system would want to build anyway when it descends its source tree during a full
# kernel build

obj-m += test.o

# all is the default action or the action that is taken if a user
# types “build all”.  In this case, it runs the make utility to
# to “make all modules in this directory” WITH the build files and
# resources in the directory containing what’s appropriate for the
# kernel you’re running.  Check out the -C option of build :)  Also
# note the use of a call to the shell to run a version of uname that
# gives the version number of the kernel.... all the better to 
# build into the module directory corresponding to the kernel that
# is actually running at the moment...

all:
   make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

# clean is the option for “cleaning up” all the complied files in
# the current build and leave just the sources.  This works
# similarly to the all: option, except that it cleans rather than
# builds.

clean:
  make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean