#include <iostream>
#include <libusb.h>

using namespace std;

void printdev(libusb_device *dev);

int main()
{
	libusb_device **devs; //pointer to pointer of device
	libusb_context *context = NULL; //a libusb session
	int returnval;
	ssize_t numDevs;
	returnval = libusb_init(&context); //initialize a library session
	
	if(returnval < 0)
	{
		cout << "Initialization Error " << returnval << endl;
		return 1;
	}

	libusb_set_debug(context, 3); //Debug verbosity level = 3 (docs)

	// saves the list of devices in dev, returns number of devices
	numDevs = libusb_get_device_list(context, &devs);

	if(numDevs < 0)
	{
		cout << "Error getting devices" << endl;
	}

	cout << numDevs << " Devices in List." << endl;

	for(ssize_t i = 0; i < numDevs; i++)
	{
		cout << i << ". " << endl;
		printdev(devs[i]); // prints device specs
	}

	int choice;
	cout << "Please Choose a device number from above: " ;
	cin >> choice;

	libusb_device *mouse = devs[choice];

	cout << "You have chosen:" << endl;
	printdev(mouse);
	
	libusb_device_handle *handle = NULL;


	returnval = libusb_open(mouse, &handle);

	if(returnval < 0)
	{
		cout << "Couldn't open device" << endl;
		return 0;
	}

	returnval = libusb_attach_kernel_driver(handle, 0);

	if(returnval < 0)
	{
		cout << "Couldn't attach drivers." << endl;
	}

	//while(true)
	//{
		


	libusb_free_device_list(devs, 1);
	libusb_exit(context);

	return 0;
}

void printdev(libusb_device *dev)
{
	libusb_device_descriptor desc;
	int r = libusb_get_device_descriptor(dev, &desc);

	unsigned char manuf[200];
	unsigned char prod[200];

	libusb_device_handle *handle = NULL;

	if(r < 0)
	{
		cout << "Failed to get the device descriptor!";
		return;
	}
	if(dev == NULL)
	{
		cout << "Device is NULL." << endl;
		return;
	}

	libusb_open(dev, &handle);

	libusb_get_string_descriptor_ascii(handle, desc.iManufacturer, manuf, 200);
	libusb_get_string_descriptor_ascii(handle, desc.iProduct, prod,200);

	cout << "Manufacturer: " << manuf << endl;
	cout << "Product Name: " << prod << endl;

	libusb_config_descriptor *config;
	libusb_get_config_descriptor(dev, 0, &config);

	cout << endl << endl;
	libusb_free_config_descriptor(config);

	libusb_close(handle);
}
