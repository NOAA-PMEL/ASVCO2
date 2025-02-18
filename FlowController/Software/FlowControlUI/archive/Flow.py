from gui.ControllerState import FlowControlInfo


def read_info(self):
    print(self.sn)
    fc_info = {
        "serial_number": self.sn,
        "version": self.version.strip("VERSION"),
        "current": int(self.current),
        "pulse": int(self.pulse),
        "count": int(self.count),
    }
    return fc_info
