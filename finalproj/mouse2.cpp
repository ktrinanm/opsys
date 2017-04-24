#include <iostream>
#include <libusb.h>
using namespace std;

void toBinary(int);

int main() {
	libusb_device **devs; //pointer to pointer of device, used to retrieve a list of devices
	libusb_context *ctx = NULL; //a libusb session
	int returnval; //for return values
	ssize_t numDev; //holding number of devices in list
	returnval = libusb_init(&ctx); //initialize a library session

	if(returnval < 0) 
	{
		cout<<"Init Error "<< returnval <<endl; //there was an error
		return 1;
	}

	libusb_set_debug(ctx, 3); //set verbosity level to 3, as suggested in the documentation
	numDev = libusb_get_device_list(ctx, &devs); //get the list of devices

	if(numDev < 0)
       	{
		cout<<"Get Device Error"<<endl; //there was an error
	}

	// open usb mouse
	libusb_device_handle *dev_handle; //a device handle
	dev_handle = libusb_open_device_with_vid_pid(ctx, 0x0461,0x4d22); //open mouse using vid and pid

	if(dev_handle == NULL)
	{
	        cout<<"Cannot open device"<<endl;
	    	//
	    	libusb_free_device_list(devs, 1); //free the list, unref the devices in it
	    	libusb_exit(ctx); //close the session
	    	return 0;
	}

	cout<<"Device Opened"<<endl;
	libusb_free_device_list(devs, 1); //free the list, unref the devices in it

	if(libusb_kernel_driver_active(dev_handle, 0) == 1) 
	{ //find out if kernel driver is attached
		cout<<"Kernel Driver Active"<<endl;
		if(libusb_detach_kernel_driver(dev_handle, 0) == 0) //detach it
		{
			cout<<"Kernel Driver Detached!"<<endl;
		}
	}

	returnval = libusb_claim_interface(dev_handle, 0); //claim interface 0 (the first) of device (mine had just 1)

	if(returnval < 0)
       	{
		cout<<"Cannot Claim Interface"<<endl;
		return 1;
	}

	cout<<"Claimed Interface"<<endl;
	int size;
	unsigned char datain[1024]="\0";
	for(int i=0;i<500;i++)
	{
		int rr = libusb_interrupt_transfer(dev_handle,
				0x81,
				datain,
				4,
				&size,
				0);
					
		for(int j=0; j<size; j++)
		{
			 toBinary(datain[j]);
		}
			 
		cout << "\n";
	}

	returnval = libusb_release_interface(dev_handle, 0); //release the claimed interface

	if(returnval != 0) 
	{
		cout<<"Cannot Release Interface"<<endl;
		return 1;
	}

	returnval = libusb_attach_kernel_driver(dev_handle, 0);

	if(returnval != 0) //detach it
	{
		cout<<"Kernel Driver Could not be Reattached to the Device."<<endl;
		return 1;
	}

	libusb_close(dev_handle);
	cout<<"Released Interface"<<endl;
	libusb_exit(ctx); //close the session

	return 0;
}
void toBinary(int a)
{
	int result[8];
	int temp =a;
	for(int i = 0; i< 8; i++)
	{
		result[i] = (temp%2);
		temp = temp/2;
	}
	for(int j = 7; j>= 0; j--)
	{
		cout << result[j];
	}
	cout << " ";

}
