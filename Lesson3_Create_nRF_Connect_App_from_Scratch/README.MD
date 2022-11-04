### Setup Project
1. To create an application using nRF Connect SDK, there are at least 3 files needed for a minimum project:
* prj.conf --> Configuration file for project
* CMakeLists.txt --> Instruction for Cmake to build source code
* src/main.c --> Main Code

2. Adding the project directory as an existing application in nRF Connect for VS Code.

Go to nRF Connect for VS Code and under the **Welcome View**, select **Add** an existing application and select the project directory. Observe the project showing up under the **Applications View**.

### Customizing the application
1. Adding custom files
* Create .c and .h files (for example: myfunction.c and .h)
* To include the custom files in the build --> use Cmake function *target_sources()*. Add the following line to CMakeLists.txt:
> target_sources(app PRIVATE src/myfunction.c)

2. Adding custom configurations 
Next, we can define our own config that will determine if our custom files get included in the build or not. 
* To do this, create a file called **Kconfig** in the application directory (the same location as **CMakeLists.txt** and **prj.conf**).
* In CMakeLists.txt, we want the addition of the custom files to be conditional. Change the last line to use the function target_sources_ifdef(), like this:
> target_sources_ifdef(CONFIG_MYFUNCTION app PRIVATE src/myfunction.c)
* Lastly, enable the config by adding the following line to prj.conf:
> CONFIG_MYFUNCTION=y

3. Modifying the devicetree
* Create an overlay file in application directory with the name of board used (for example: nrf52840dk_nrf52840.overlay)
* Define the new setup for device tree on the overlay file. For example, we want to change the baud rate for the UART instance used when printing to console, which is uart0. Add these following lines to the overlay file:
> &uart0 {
> 	current-speed = <9600>;
> };




