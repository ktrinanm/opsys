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

	cout << "Number of possible configurations: "
		<< (int) desc.bNumConfigurations << endl;
	cout << "Device Class: " << (int)desc.bDeviceClass << endl;
	cout << "VendorID: " << desc.idVendor << endl;
	cout << "ProductID: " << desc.idProduct << endl;
	
	libusb_config_descriptor *config;
	libusb_get_config_descriptor(dev, 0, &config);

	cout << "Interfaces: " << (int)config->bNumInterfaces << " ||| ";

	const libusb_interface *inter;
	const libusb_interface_descriptor *interdesc;
	const libusb_endpoint_descriptor *epdesc;

	for(int i = 0; i < (int)config->bNumInterfaces; i++)
	{
		inter = &config->interface[i];

		cout << "Number of alternate settings: "
			<< inter->num_altsetting << " | ";

		for(int j = 0; j < inter->num_altsetting; j++)
		{
			interdesc = &inter->altsetting[j];
			cout << "Interface Number: "
				<< (int)interdesc->bInterfaceNumber
				<< " | ";
			cout << "Number of endpoints: "
				<< (int)interdesc->bNumEndpoints
				<< " | ";
			
			for(int k = 0; k < (int)interdesc->bNumEndpoints;
					k++)
			{
				epdesc = &interdesc->endpoint[k];
				cout << "Descriptor Type: "
					<< (int)epdesc->bDescriptorType 
					<< " | ";
				cout << "EP Address: "
					<< (int)epdesc->bEndpointAddress 
					<< " | ";
			}
		}
	}

	cout << endl << endl << endl;
	libusb_free_config_descriptor(config);
}
