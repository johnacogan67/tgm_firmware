# tgm_firmware

Firmware for the TGM

# nRF Connect SDK example application

This repository is based on [ncs-example-application](https://nrfconnect.github.io/ncs-example-application)
which contains an nRF Connect SDK example application. The main
purpose of this repository is to serve as a reference on how to structure nRF Connect
SDK based applications. Please refer to the documentation in that repository for information related to the structuring of this repo.

## Getting started

Before getting started, make sure you have a proper nRF Connect SDK development environment.
Follow the official
[Installation guide](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/installation/install_ncs.html).

### Initialization

The first step is to initialize the workspace folder (`tgm_firmware`) where
the `tgm_firmware` and all nRF Connect SDK modules will be cloned. Run the following
command:

```shell
# initialize tgm_firmware workspace for the tgm_firmware application (main branch)
west init -m https://github.com/johnacogan67/tgm_firmware.git --mr main tgm_firmware
# update nRF Connect SDK modules
cd tgm_firmware
west update
```

### Building and running

The easiest way to build and run the example is using the nRF Connect extension in VSCode.
When freshly installing this repo, you will need to 'Add existing application' and select the tgm_firmware/app directory.
Next, add a build configuration, selecting the board you want to build for.
Check the boards directory for a selection of supported boards

Give the 'Build directory name' a good name to distinguish different builds (e.g. add board as a suffix)
Ensure that 'Build after generating configuration' is checked and click on 'Build configuration'

For future builds, you can use the 'Build' action in the nRF Connect pane.
Debugging (make sure to select Optimization level 'Optimize for debugging -Og')) and Flashing a device can also be done through here
