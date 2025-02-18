#!/bin/python

from retry import retry
from serial import Serial
from enum import Enum
import time


# Modes
MODES = [
    "ZPON",
    "ZPOFF",
    "ZPPCAL",
    "ZPVENT",
    "SPON",
    "SPOFF",
    "SPVENT",
    "SPPC",
    "EPON",
    "EPOFF",
    "EPVENT",
    "EPPOST",
    "APON",
    "APOFF",
    "APPOST",
    "REST",
    "DEPLOY",
    "PRES",
    "PURGE",
    "CLEAR",
]


class NackError(ValueError):
    """Recieved NACK instead of ACK"""

    pass


class InfoTypes(Enum):
    """Flow Controller interface commands

    Args:
        Enum (_type_): Info command
    """
    VERSION = "ver"
    PULSE = "pulse"
    CURRENT = "current"
    COUNT = "count"
    MODE = "mode"


INFO_DICT = {
    "ver": "VERSION",
    "pulse": "PULSE",
    "current": "CURRENT",
    "count": "COUNT",
    "mode": "MODE",
}


class FlowControl:
    """Flow Control Class"""
    def __init__(self, port):
        self.serial = Serial()
        self.serial.port = port
        self.serial.baudrate = 9600
        self.serial.timeout = 2

    def __enter__(self):
        self.serial.open()

    def __exit__(self, exc_type, exc_value, trace):
        self.serial.close()

    def flush(self):
        """Flush the serial port"""
        self.serial.flush()

    def open(self):
        """Open the serial port"""
        self.serial.open()
        self.serial.write(b"\r\n")
        time.sleep(1)
        self.flush()

    def close(self):
        """Close the serial port"""
        self.serial.close()

    def write(self, value):
        """Write a value to the serial port
        Args:
            value (str): Value to write
        """
        self.flush()
        value += "\r\n"
        self.serial.write(value.encode())
        self.serial.flush()

    def read(self) -> str:
        """Read a value from the serial port
        Returns:
            str: Value read
        """
        self.flush()
        value = self.serial.readline()
        assert len(value) > 0
        value = value.decode("utf8")
        value = value[: value.find("\r\n")]
        value = value[value.find("\x06") + 1 :]
        self.serial.flush()
        return value

    def set_mode(self, mode_name: str):
        """Set the Flow Control Mode

        Args:
            mode_name (str): Mode identifier (i.e. EPOFF, SPON, etc.)

        Raises:
            ValueError: Invalide mode provided
        """
        if mode_name not in MODES:
            raise ValueError("Invalid mode")

        send_str = f"mode={mode_name}"
        self.write(send_str)

    # @property
    # def mode(self) -> str:
    #     self.flush()
    #     self.write("mode")
    #     value = self.read()
    #     value = self._strip_response(value, "MODE")
    #     return value

    # @mode.setter
    # def mode(self, mode_name="DEPLOYMENT"):
    #     if mode_name not in MODES:
    #         raise ValueError("Invalid mode")

    #     send_str = f"mode={mode_name}"
    #     self.write(send_str)

    def set_valve(self, valve: int, direction: str):
        """Set an individual valve

        Args:
            valve (int): Valve to set
            direction (str): 'ac' or 'bc'

        Raises:
            ValueError: _description_

        Returns:
            None
        """
        if valve < 1 or valve > 6:
            raise ValueError(f"Invalid valve: {valve}.  Must be between 1-6")

        valve -= 1  # Convert to index correctly on device (Valve number - 1)
        send_str = ""
        if direction == "ac" or direction == "ca":
            send_str = f"vac={valve}"
        elif direction == "bc" or direction == "cb":
            send_str = f"vbc={valve}"
        else:
            raise ValueError(
                f'Invalid direction: {direction}.  "ac" and "bc" are only options'
            )

        self.write(send_str)

    # @property
    # def pulse(self):
    #     self.flush()
    #     self.write("pulse")
    #     value = self.read()
    #     value = self._strip_response(value, "PULSE")
    #     value = int(value)
    #     return value

    def set_pulse(self, pulse_length: int):
        """Set the pulse length

        Args:
            pulse_length (int): Length of pulse in milliseconds

        Raises:
            ValueError: Pulse length greater than manufacterer recommendation
            ValueError: Pulse length shorter than manufacterer recommends to actuate
        """
        if pulse_length > 100:
            raise ValueError(
                f"LEE Valves recommends no more than 100ms for these valves.  {pulse_length}ms was requested"
            )
        if pulse_length < 10:
            raise ValueError(
                f"{pulse_length} may result in no actuation.  LEE Valves recommends a pulse length of 10-100ms  "
            )

        send_str = f"pulse={pulse_length}"
        self.write(send_str)
        time.sleep(0.5)  #

    # @pulse.setter
    # def pulse(self, pulse_length: int):
    #     if pulse_length > 100:
    #         raise ValueError(
    #             f"LEE Valves recommends no more than 100ms for these valves.  {pulse_length}ms was requested"
    #         )
    #     if pulse_length < 10:
    #         raise ValueError(
    #             f"{pulse_length} may result in no actuation.  LEE Valves recommends a pulse length of 10-100ms  "
    #         )

    #     send_str = f"pulse={pulse_length}"
    #     self.write(send_str)
    #     time.sleep(0.5)  # Maybe change to look for ACK

    # @property
    # def current(self):
    #     self.flush()
    #     self.write("current")
    #     value = self.read()
    #     value = self._strip_response(value, "CURRENT")
    #     value = int(value)
    #     return value

    def set_current(self, current: int):
        """Set the H-Bridge current value

        Args:
            current (int): Set the H-Bridge current value to (1-7)

        Raises:
            ValueError: Invalid entry
        """
        if current < 0 or current > 7:
            raise ValueError("current value must be betwen 0 and 7")

        send_str = f"current={current}"
        self.write(send_str)
        time.sleep(0.5)

    # @current.setter
    # def current(self, current: int):
    #     if current < 0 or current > 7:
    #         raise ValueError("current value must be betwen 0 and 7")

    #     send_str = f"current={current}"
    #     self.write(send_str)
    #     time.sleep(0.5)  # Maybe change to look for ACK

    # @property
    # def version(self):
    #     self.flush()
    #     self.write("ver")
    #     return self.read()



    # @property
    # def sn(self):
    #     self.flush()
    #     self.write("ser")
    #     value = self.read()
    #     value = self._strip_response(value, "SERIAL")
    #     return value

    # @property
    # def count(self):
    #     try:
    #         self.flush()
    #         self.write("count")
    #         value = self.read()
    #         value = self._strip_response(value, "COUNT")
    #         value = int(value)
    #     except ValueError:
    #         raise NackError

    #     return value

    def get_valves(self)->list[str]:
        """Get a list of the currentvalve settings

        Returns:
            list[str]: List of string settings
        """
        invalid = True
        while invalid:
            try:
                self.write("\n")
                data = [self.read()[-6:]]
            except ValueError:
                print("try again")
            finally:
                invalid = False
        return [*data[0]]

    @retry(NackError, delay=0.5, tries=5)
    def get_info(self, info: InfoTypes)-> str:
        """Get system information

        Args:
            info (InfoTypes): Read the info for the type specified

        Raises:
            NackError: If a nack is recevied, retry runs

        Returns:
            str: Value of info specified
        """
        time.sleep(0.5)
        try:
            self.flush()
            self.write(info.value)
            value = self.read()
            value = self._strip_response(value, INFO_DICT[info.value])
            if (
                info == InfoTypes.CURRENT
                or info == InfoTypes.PULSE
                or info == InfoTypes.COUNT
            ):
                value = int(value)
        except ValueError:
            raise NackError

        return value

    def clear_valves(self):
        """Set the valve clearing state
        """
        self.write("panic")

    def _strip_response(self, value: str, identifier: str = "")->str:
        """Strip the response from a reading

        Args:
            value (str): value read from device
            identifier (str, optional): String to be stripped (+ the :). Defaults to "".

        Returns:
            str: stripped response
        """
        val = value[value.find(identifier + ":") + len(identifier) + 1 :]
        val = val.strip(" ")
        return val


if __name__ == "__main__":
    import time

    try:
        fc = FlowControl("COM12")
        fc.open()
        time.sleep(1)

        for i in range(1, 7):
            fc.valve(i, "ac")
            time.sleep(1)
            fc.valve(i, "bc")
            time.sleep(1)
    except Exception as e:
        raise (e)

    finally:
        fc.mode = "DEPLOY"
        fc.close()
