from dataclasses import dataclass


@dataclass
class FlowControlInfo:
    serial_number: str
    version: str
    current: int
    pulse: int
    count: int
