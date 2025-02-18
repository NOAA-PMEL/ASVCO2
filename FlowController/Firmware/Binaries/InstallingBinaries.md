# MSP430 Programming Instructions

PMEL's Engineering Development Division uses MSP430 microcontrollers in many of
the custom circuits.  The following is the procedure for programming the
microcontrollers, also known as "flashing".

## Requirements
The following are the basic hardware and software requirements for loading
binary files onto the microcontroller.

### Hardware
* Texas Instruments MSP-FET Flash Emulation Tool
* EDD Custom 0.100" Pitch x14-pin to 0.050" pitch x10-pin converter

### Software
* Elprotronic, Inc. FET-Pro-430-LITE
  * Free download from www.elprotronic.com

### Files
* Pre-generated binary files for target device
  * File should have the form "DeviceName_version.txt


## Loading "Binary" files to target
  1. Connect the programming cable to the microcontroller and the MSP-FET.

  2. Apply Power to the target board

  3. Open the FET-Pro-430 program

  4. Press the button "Open Code File ->", navigate to the location of the "binary"
  file (ProjectName.txt)

    * In the source code project folder, this is found in Debug->Exe->Projectname.txt

    ![](Images/FET_Pro430_Step2.jpg)



  5. In the Microcontroller Type block (refer to User Manual for values)
    * In "Group", select the general type of MSP430 used.
    * On the next line, select the specific type of MSP430 used

    ![](Images/FET_Pro430_Step2.png)

  6. In the "Device Action" block, press "AUTO PROG."
    * If successfull, the "Status" block should show **"Pass"**
    * If unsuccessfull, press "ERASE FLASH" and try step 6 again.

    ![](Images/FET_Pro430_Step3.png)

  7. The device is now programmed

    * **NOTE:** The board may require additional configuration settings over the
    console.  Consult the User Manual for the corresponding device.
