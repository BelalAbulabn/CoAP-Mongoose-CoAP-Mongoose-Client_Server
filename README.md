# CoAP Client and Server Example

This repository contains example code for a Constrained Application Protocol (CoAP) client and server. CoAP is a specialized web transfer protocol for use with constrained nodes and constrained networks in the Internet of Things. The CoAP client is written in C, and the CoAP server is written in Python.

## Getting Started

### Prerequisites

- For the CoAP client, you'll need a C compiler such as GCC.
- For the CoAP server, you'll need Python 3.x.

### Cloning the Repository

To clone the repository, use the following command:
git clone <repository-url>
  
Replace `<repository-url>` with the URL of this repository.

## CoAP Client

The CoAP client is implemented in C and uses the Mongoose and lwIP libraries. It can be used to send CoAP requests to a CoAP server.

### Building the Client

Navigate to the client directory and use the makefile to build the client.

```sh
cd client_directory
make
```
### Running the Client

After building, you can run the client using:

```sh
./client_executable
```
## CoAP Server
The CoAP server is implemented in Python using the CoAPthon library. It exposes resources such as temperature, light, and button states over CoAP.

### Installing Dependencies
Install the CoAPthon library using pip:
```sh
pip install CoAPthon
```
### Running the Server
Navigate to the server directory and run the server using Python:
```sh
cd server_directory
python server.py
```
The server will start and listen on the default CoAP port (5683).
 ## Resources

The server exposes the following resources:

- `/temp` - Represents temperature in Celsius.
- `/light` - Represents the state of a light (on/off).
- `/button` - Represents the state of a button (on/off).

Clients can perform CoAP GET, PUT, and POST requests on these resources.

## Authors

- Belal Abulabn

## Acknowledgments

- Mongoose library
- lwIP library
- CoAPthon library
