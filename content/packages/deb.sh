#!/bin/bash

#
#   This file is part of Magnum.
#
#   Copyright © 2010, 2011, 2012, 2013, 2014, 2015, 2016, 2017, 2018, 2019,
#               2020, 2021, 2022, 2023 Vladimír Vondruš <mosra@centrum.cz>
#   Copyright © 2024 Dora "catdotjs" <catdotjs@gmail.com>
#
#   Permission is hereby granted, free of charge, to any person obtaining a
#   copy of this software and associated documentation files (the "Software"),
#   to deal in the Software without restriction, including without limitation
#   the rights to use, copy, modify, merge, publish, distribute, sublicense,
#   and/or sell copies of the Software, and to permit persons to whom the
#   Software is furnished to do so, subject to the following conditions:
#
#   The above copyright notice and this permission notice shall be included
#   in all copies or substantial portions of the Software.
#
#   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
#   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
#   DEALINGS IN THE SOFTWARE.
#

# Packages
builddeps=("git" "dpkg-dev" "debhelper" "cmake" "libgl-dev" "libopenal-dev" "libglfw3-dev" "libsdl2-dev")
basegits=("corrade" "magnum")

# Other packages dependencies
integrationdeps=("libbullet-dev" "libeigen3-dev" "libglm-dev")
pluginsdeps=("libdevil-dev" "libjpeg-dev" "libpng-dev" "libfreetype-dev" "libwebp-dev" "libfaad-dev" "libassimp-dev" "libopenexr-dev" "spirv-tools" "glslang-dev" "glslang-tools")
extrasdeps=() # for now nothing
examplesdeps=("libbox2d-dev" "libbullet-dev" "libopenal-dev")

# Flags
mintegration=1
mplugins=2
mextras=4
mexamples=8
everythingflag=15

# This is used to store the flags
installflags=0

skipaskinginstall=false

leave() {
  echo "Exiting..."
  exit
}

# $1 Name of the package
git_install() {
  git clone https://github.com/mosra/$1 && cd $1
  ln -s package/debian .
  dpkg-buildpackage --no-sign
  sudo dpkg -i ../$1*.deb
}

# $1 Name of the package
# $2 Installation media - "apt" or "git"
package_installer() {
  case $2 in
  apt)
    sudo apt install $1 -y
    ;;

  git)
    git_install "$1"
    ;;
  *)
    echo "Installation script tried to install with $2. Which is unaccounted for!"
    leave
    ;;
  esac
}

# $1 Name of the package
# $2 Installation media - "apt" or "git"
ask_to_install() {
  echo "Do you want to install $1 (This package is needed to continue the installation)? (y/n)"
  read -r resp
  if [[ "$resp" == "y" || "$resp" == "Y" ]]; then
    package_installer "$1" "$2"
  elif [[ "$resp" == "n" || "$resp" == "N" ]]; then
    echo "$1 needs to installed for installer to continue!"
    leave
  else
    echo "Please only respond with y/n"
    ask_to_install "$1" "$2"
  fi
}

# $1 Name of the package
# $2 Installation media - "apt" or "git"
# $3 Should you force install - true or false
package_check() {
  if dpkg-query -W -f='${Status}' $1 2>/dev/null | grep -q "install ok installed"; then
    echo "$1 is OK ($(dpkg-query -W -f='${Version}' $1))."
  else
    echo "$1 is missing!"
    if [[ $3 = true || $skipaskinginstall = true ]]; then
      package_installer "$1" "$2"
    else
      ask_to_install "$1" "$2"
    fi
  fi
}

base_installer() {
  echo "Do you want to be asked for every package being installed? (y/n)"
  read -r option
  if [[ "$option" == "n" || "$option" == "N" ]]; then
    skipaskinginstall=true
  fi

  printf "\n-- Installing dependencies to build --\n"
  for d in ${builddeps[*]}; do
    package_check $d "apt"
  done

  printf "\n-- Installing the base packages -- \n"
  for pkg in ${basegits[*]}; do
    package_check $pkg "git" true
  done
}

begin_custom_installer() {
  # Just making sure base is installed. It won't do anything if it is already.
  base_installer
  if ((installflags & mintegration)); then
    for d in ${integrationdeps[*]}; do
      package_check $d "apt"
    done
    package_check "magnum-integration" "git" true
  fi

  if ((installflags & mplugins)); then
    for d in ${pluginsdeps[*]}; do
      package_check $d "apt"
    done
    package_check "magnum-plugins" "git" true
  fi

  if ((installflags & mextras)); then
    for d in ${extrasdeps[*]}; do
      package_check $d "apt"
    done
    package_check "magnum-extras" "git" true
  fi

  if ((installflags == everythingflag)); then
    for d in ${examplesdeps[*]}; do
      package_check $d "apt"
    done
    package_check "magnum-examples" "git" true
  elif ((installflags & mexamples)); then
    echo "Skipping installing magnum-examples. Please select everything before installing examples!"
  fi
  echo "Installation finished!"
  leave
}

# $1 specific flag to check
check_if_selected() {
  if ((installflags & $1)); then
    echo "X"
  else
    echo " "
  fi
}

# $1 name of the package
check_if_installed_already() {
  if dpkg-query -W -f='${Status}' $1 2>/dev/null | grep -q "install ok installed"; then
    echo "(already installed. $(dpkg-query -W -f='${Version}' $1))"
  fi
}

# It would be neat if instead of re-writing it every selection. Cursour could just move to
# where the change happened. I was too tired to write that but I will definetly come back to it
#
# Additionally flag system might be bit too much for this simple thing lol
custom_installer_menu() {
  echo "[X] - Base Packages (Magnum and Corrade)"
  echo "[$(check_if_selected mintegration)] - 1 - Magnum-integration $(check_if_installed_already "magnum-integration")"
  echo "[$(check_if_selected mplugins)] - 2 - Magnum-plugins $(check_if_installed_already "magnum-plugins")"
  echo "[$(check_if_selected mextras)] - 3 - Magnum-extras $(check_if_installed_already "magnum-extras")"
  echo "[$(check_if_selected mexamples)] - 4 - Magnum-examples (Requires every package to be installed!) $(check_if_installed_already "magnum-examples")"
  echo "Enter 'y' to start installation. 'n' to go back. 'q' to exit."
  read -r option

  case $option in
  1)
    installflags=$((installflags ^ mintegration))
    custom_installer_menu
    ;;
  2)
    installflags=$((installflags ^ mplugins))
    custom_installer_menu
    ;;
  3)
    installflags=$((installflags ^ mextras))
    custom_installer_menu
    ;;
  4)
    installflags=$((installflags ^ everythingflag))
    custom_installer_menu
    ;;
  y)
    begin_custom_installer
    ;;
  n)
    installflags=0
    menu_choosing
    ;;
  q)
    leave
    ;;
  *)
    echo "The given input is not valid, please try again!"
    custom_installer_menu
    ;;
  esac
}

# I was lazy so I decided to basically set every flag on installflags and then run begin_custom_installer
everything_installer() {
  echo "installing everything!"
  installflags=everythingflag
  begin_custom_installer
}

menu_choosing() {
  echo "Please choose which one of the following you would like to install.\n"
  echo "1 - Base (Magnum and Corrade only) installer"
  echo "2 - Base + \"choose what you want\" installer"
  echo "3 - Everything installer"
  echo "q - Exit"
  read -r option

  case $option in
  1)
    base_installer
    echo "Installation finished"
    leave
    ;;

  2)
    echo "Following additional packages could be installed with Magnum/Corrade"
    custom_installer_menu
    ;;
  3)
    everything_installer
    ;;
  q)
    leave
    ;;
  *)
    echo "The given input is not valid, please try again!"
    menu_choosing
    ;;
  esac
}

# Script starts here
echo "-- Written by catdotjs@github 2024 --"
echo "-- updating apt --"
sudo apt update
echo "-- updated apt for latest packages --"
cd $(mktemp -d)
echo "-- Current working directory is $(pwd) --"
echo "-- Installer version 1.0.1 --"
printf "Welcome to the Magnum/Corrade installer!\nCheck out https://magnum.graphics/ and https://doc.magnum.graphics/ to learn more.
\n"
menu_choosing
