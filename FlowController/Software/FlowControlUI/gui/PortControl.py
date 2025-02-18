import serial.tools.list_ports


def read_ser_ports():
    ports = serial.tools.list_ports.comports()
    return [port.name for port in ports]
