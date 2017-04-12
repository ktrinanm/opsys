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
		printdev(devs[i]); // prints device specs
	}

	libusb_free_device_list(devs, 1);
	libusb_exit(context);

	return 0;
}

void printdev(libusb_device *dev)
{
	libusb_device_descriptor desc;
	int r = libusb_get_device_descriptor(dev, &desc);

	if(r < 0)
	{
		cout << "Failed to get the device descriptor!";
		return;
	}

	unsigned char *manuf;
	libusb_device_handle *handle = NULL;

	if(dev == NULL)
	{
		cout << "Device is NULL." << endl;
		return;
	}

	libusb_open(dev, &handle);

	cout << "Number of possible configurations: "
		<< (int) desc.bNumConfigurations << endl;
	cout << "Device Class: " << (int)desc.bDeviceClass << endl;
	cout << "VendorID: " << desc.idVendor << endl;
	cout << "ProductID: " << desc.idProduct << endl;

	libusb_get_string_descriptor_ascii(handle, desc.iManufacturer, manuf, 200);

	cout << "Manufacturer: " << manuf << endl;

	libusb_config_descriptor *config;
	libusb_get_config_descriptor(dev, 0, &config);

	cout << endl << endl << endl;
	libusb_free_config_descriptor(config);
}
