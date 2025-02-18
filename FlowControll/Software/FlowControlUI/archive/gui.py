import streamlit as st
from src.FlowController import FlowControl
from src.ControllerState import FlowControlInfo


def read_info(fc: FlowControl):
    print(fc.sn)
    fc_info = FlowControlInfo(
        serial_number=fc.sn,
        version=fc.version.strip("VERSION"),
        current=int(fc.current),
        pulse=int(fc.pulse),
        count=int(fc.count),
    )
    return fc_info


def read_ser_ports():
    ports = serial.tools.list_ports.comports()
    return [port.name for port in ports]


def update_sidebar():
    st.title("Flow Controller Dashboard")
    st.header("Serial Port")
    port_select = st.multiselect("Select Serial Port", ports)
    port_btn = st.button("Connect to port")
    if port_btn:
        fc = FlowControl(port_select[0])
        fc.open()
        print(f"Connected to {port_select[0]}")
        serial_flag = True
        fc_info = read_info(fc)


# START OF STREAMLIT
serial_flag = False
fc_info = None
ports = read_ser_ports()


st.header("Individual Valve Control")
c1, c2, c3, c4, c5, c6 = st.columns(6)
# def set_valve(valve:int, direction:str):
if serial_flag:
    b1a = c1.button(
        label="V1:ac",
        key="v1a",
        on_click=fc.set_valve,
        kwargs={"valve": 1, "direction": "ac"},
    )

    b1b = c1.button(
        label="V1:bc",
        key="v1b",
        on_click=fc.set_valve,
        kwargs={"valve": 1, "direction": "bc"},
    )


st.header("Mode Control")


st.header("Flow Controller Info")
if fc_info:
    st.write(f"Serial Number: {fc_info.serial_number}")
    st.write(f"Version: {fc_info.version}")
    st.write(f"Current: {fc_info.current}")
