#!/bin/sh

insmod -x -y getroot.o
insmod unlink.o
rmmod unlink
