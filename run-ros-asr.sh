#!/bin/bash

cd src
gnome-terminal -e 'sh -c (../bin/listener)'
(../bin/talker)
