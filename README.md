# LM15SGFNZ07 library

## Overview
Raspberry Pi library for LM15SGFNZ07 display that comes with the following Siemens phones: M55, S55, C60, MC60, A65.

The library is based on:
- [GPIO LM15SGFNZ07 library for Arduino](https://bitbucket.org/mindogas/lm15sgfnz07),
- [SPI LM15SGFNZ07 library for Arduino](https://github.com/DhrBaksteen/Arduino-SPI-LM15SGFNZ07-LCD-Library),

and has been ported to Raspberry Pi.

## Installation
Make sure `git` is installed on the Raspberry Pi along with the latest packages:
```
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install -y git
```
Install all the LM15SGFNZ07 library and examples dependencies:
```
sudo apt-get install -y wiringpi libpng-dev libcurl4-nss-dev libjansson-dev
```
Download the git respository:
```
git clone https://github.com/marcin-sielski/LM15SGFNZ07.git
```
Build the library and examples:
```
cd LM15SGFNZ07
make
```
In order to be able to run [SmarThermo](https://github.com/marcin-sielski/LM15SGFNZ07/tree/master/examples/SmarThermo)
example create an account on [Open Weather Map](http://openweathermap.org/) service, obtain the API key and create
`/home/pi/.SmarThermo` file with the following content:
```
ApiKey XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
Location CityName,CountryCode      
```
and enable 1-wire interface with `raspi-config`.

Install the library and the examples:
```
sudo make install
```
