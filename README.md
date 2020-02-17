# IoT-Ticket Linux C++ client

IoT-Ticket C++ client provides an easy-to-use library and examples so that Your application can take advantage of the versatile IoT-Ticket cloud tools.

## System requirements

This C++ library is intended to be used in a Linux based system. The communication with the IoT-Ticket server is implemented using libcurl library which is found in most Linux systems. The project uses CMake for build so the library can be easily built for both PC and Embedded environments.

This project does not build the libcurl and it needs to be installed before building the IoT-Ticket client. Moreover, if the tests are built, cppunit needs to be installed also.

## Getting started
1. Create your own IoT-Ticket account at https://www.iot-ticket.com/ (Request an invitation)
2. Wait for the account activation email
3. Build the library
4. Start using it in your application

### Building the library

The library project uses CMake. To build the library for the host system, following commands can be issued from the project root directory:
```sh
$ cmake iot-ticket-client
$ make
```

These commands build the shared library. Additionally tests and a demo application can be built. The tests and demo application use features from C++11 standard which needs to be enabled by providing a compiler flag. For older compilers the correct flag might be "-std=c\+\+0x"
```sh
$ cmake -DBUILD_TESTS=1 -DBUILD_DEMO=1 -DCMAKE_CXX_FLAGS="-std=c++11 -lpthread" iot-ticket-client
$ make
```

### Example code

The library contains a demo which provides a complete example application. Also, the unit tests can be used as a reference.
```
iot-ticket-api/demo.cpp
iot-ticket-api/tests/IOT_Tester.cpp
```

Once the demo project has been built, it can be started from the command line:
```sh
$ iot-ticket-demo -u USER -p PASS
```
The demo code will register a new device to the respective user and log load average, free RAM and number of processes from the machine (similar data that is shown in 'top' utility). On subsequent calls the device id can be given from the command line, and the demo application uses the existing device.
```sh
$ iot-ticket-demo -u USER -p PASS -d DEVICE_ID
```

### Registering a device

```cpp
std::string user = "<your_user>";
std::string pass = "<your_pass>";
std::string devID;

IOT_API api("https://my.iot-ticket.com/api/v1/", user, pass);

IOT_RegDevice device;
device.SetName("Device Name");
device.SetManufacturer("Manufacturer Name");
device.AppendAttribute("Key1", "Value1");
if(api.RegisterDevice(device, devID) != IOTAPI::IOT_ERR_OK) {
    // error;
}
// use 'devID' in subsequent queries
```

### Sending data
```cpp
IOT_WriteData data;
data.SetName("Temperature");
data.SetPath("Engine");
data.SetUnit("C");

data.SetValue(87.5);
data.SetTimeToNow();

if(api.SendData(devID, data) != IOTAPI::IOT_ERR_OK) {
	// error
}
```
### Get datanodes for a device
```cpp
std::vector<IOT_ReadData> datanodes;

if(api.GetDatanodes(m_devId, datanodes) != IOTAPI::IOT_ERR_OK) {
	// error
}
```

### Read data
```cpp
IOT_ReadDataFilter filter;
filter.AddDatanode("NodeName");
filter.SetFromDate(12345);
filter.SetLimit(100);

std::vector<IOT_ReadData> data;
if(api.ReadData(m_devId, filter, data) != IOTAPI::IOT_ERR_OK) {
	// error
}
```

## API documentation
This C++ client library uses the IoT-Ticket REST API. The documentation for the underlying REST service can be found from
https://www.iot-ticket.com/images/Files/IoT-Ticket.com_IoT_API.pdf
