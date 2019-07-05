
	Author   KMS - Martin Dubois, ing.
	Product  FX3-KB
	File	 README.md

This project use a FX3 SuperSpeed Explorer Kit to simulate a USB keyboard and
control a computer.

For more information about the FX3 SuperSpeed Explorer Kit, visite

https://www.cypress.com/documentation/development-kitsboards/cyusb3kit-003-ez-usb-fx3-superspeed-explorer-kit

or

https://www.digikey.ca/en/product-highlight/c/cypress/ez-usb-fx3-superspeed-explorer-kit?utm_adgroup=General&mkwid=sd6iqWqhU&pcrid=267468163121&pkw=&pmt=b&pdv=c&productid=&slid=&gclid=CjwKCAjw6vvoBRBtEiwAZq-T1Zqos_NLTk5cFeKQbb-C23-dVT4Kqt_SUK8HmqtT8jIkP-bavAX7CxoCLMYQAvD_BwE

===== Development computer preparation - Firmware ===========================

    [ ] Install the EZ-USB FX3 Software Development Kit version 1.3.4

You can download the EZ-USB Software Development Kit at

https://www.cypress.com/documentation/software-and-drivers/ez-usb-fx3-software-development-kit

===== Development computer preparation - Hardware ===========================

    [ ] Install OpenSCAD 2019.05 by executing
        K:\Logiciels\OpenSCAD\OpenSCAD-2019.05-x86-640-Installer.exe

You can download OpenSCAD at

https://www.openscad.org/downloads.html

===== Development computer preparation - Software ===========================

Soon!

===== After getting the GIT repository - Firmware ===========================

    [ ] Tool "EZ-USB suite"
        - Import the project from the "Firmware" sub-folder

===== Compilation - Firmware ================================================

    [ ] Tool "EZ-USB suite"
        - In the left, select the project
        - In the menu "Project", select "Build"

===== Compilation - Hardware ================================================

    [ ] Tool "OpenSCAD"
        - Open "Hardware/Base.scad"
        - Render
        - Generate STL
        - Open "Hardware/Top.scad"
        - Render
        - Generate STL

===== Programming the I2C EEPROM of the FX3 SuperSpeed Explorer Kit =========

    [ ] Put a jumber on the PMODE (J4) connector
    [ ] Connect the FX3 SuperSpeed Explorer Kit to the development computer
        using the USB 3.0 port
    [ ] Tool - "Cypress Control Center"
        - In the left part, select the device
        - In the menu, select "Program the I2C EEPROM"
        - Select the file "Firmware\TODO.img"
        - Wait until the status bar indicates the completion of the operation
