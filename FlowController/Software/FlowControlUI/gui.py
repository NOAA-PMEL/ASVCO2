""" Streamlit Program to Control the ASVCO2 Flow Control Board"""
import streamlit as st
from src.FlowController import FlowControl, MODES, InfoTypes
from gui.PortControl import read_ser_ports
import pandas as pd


def read_info(fc: FlowControl):
    """Read the info off of the Flow Controller"""
    fc_info = {
        "version": fc.get_info(InfoTypes.VERSION),
        "current": int(fc.get_info(InfoTypes.CURRENT)),
        "pulse": int(fc.get_info(InfoTypes.PULSE)),
        "count": int(fc.get_info(InfoTypes.COUNT)),
    }
    return fc_info


def draw_valves():
    """Draw the individual valve control buttons"""
    cols = st.columns(6)
    fc = st.session_state.controller
    for idx, col in enumerate(cols):
        vIdx = idx + 1
        v1_ac = col.button(
            label=f"V{vIdx}:ac",
            key=f"v{vIdx}a",
            on_click=fc.set_valve,
            kwargs={"valve": vIdx, "direction": "ac"},
        )
        v1_bc = col.button(
            label=f"V{vIdx}:bc",
            key=f"v{vIdx}_bc",
            on_click=fc.set_valve,
            kwargs={"valve": vIdx, "direction": "bc"},
        )


def draw_modes():
    """Draw the Flow Controller mode buttons"""
    cols = st.columns(6)
    fc = st.session_state.controller
    for idx, mode in enumerate(MODES):
        idxVal = (idx) % 5
        cols[idxVal].button(
            label=mode,
            key=f"{mode}_btn",
            on_click=fc.set_mode,
            kwargs={"mode_name": mode},
        )


def draw_valve_state():
    """Draw the valve state components"""
    st.header("Current Valve Settings")
    cols = list(st.columns(6))

    for idx, state in enumerate(st.session_state.valve_state):
        # delta = 1 if state == 'a' else -1
        delta = None
        if state != st.session_state.valve_state_last[idx]:
            if state.upper() == "A":
                delta = 1
            elif state.upper() == "B":
                delta = -1

        cols[idx].metric(label=f"V{idx+1}", value=state, delta=delta)


def draw_info():
    """Create the sidebar System Info objects"""
    ct = st.sidebar.container()
    current = ct.number_input(
        label="Set Current",
        min_value=1,
        max_value=7,
        value=st.session_state.info["current"],
    )
    pulse = ct.number_input(
        label="Set Pulse",
        min_value=10,
        max_value=100,
        value=st.session_state.info["pulse"],
    )
    ct.number_input(label="Count", value=st.session_state.info["count"], disabled=True)
    ct.text_input(
        label="Firmware", value=st.session_state.info["version"], disabled=True
    )

    update_btn = st.sidebar.button(label="Update", key="update_btn")
    if update_btn:
        if current:
            st.session_state.controller.set_current(current)
        if pulse:
            st.session_state.controller.set_pulse(pulse)


def draw_sidebar():
    """Draw the sidebar components"""
    ports = read_ser_ports()
    port_select = st.sidebar.selectbox("Select Port", options=ports)
    connect_btn = st.sidebar.button("Connect", disabled=st.session_state.connect_btn)
    disconnect_btn = st.sidebar.button(
        "Disconnect", key="dis_btn", disabled=st.session_state.disconnect_btn
    )
    if connect_btn:
        st.session_state.port_connected = True
        st.session_state.disconnect_btn = False
        st.session_state.connect_btn = True
        with st.spinner():
            fc = FlowControl(port_select)
            fc.open()
            st.session_state.controller = fc

        st.session_state.info = read_info(fc)

    if disconnect_btn:
        st.session_state.port_connected = False
        st.session_state.disconnect_btn = True
        st.session_state.connect_btn = False
        st.session_state.controller.close()
        fc = None
        st.session_state.info = None

    st.sidebar.header("System Info")
    if st.session_state.info:
        draw_info()


def app():
    """Application (draws all subsystems)"""
    fc = st.session_state.controller

    # Current Valve Settings
    draw_valve_state()

    # Individual Valves
    st.header("Individual Valve Control")
    draw_valves()

    # Set Modes
    st.header("Set Mode")
    draw_modes()


def main():
    """Main Program"""
    fc = None

    if "port_connected" not in st.session_state:
        st.session_state.port_connected = False
    if "disconnect_btn" not in st.session_state:
        st.session_state.disconnect_btn = True
    if "connect_btn" not in st.session_state:
        st.session_state.connect_btn = False
    if "controller" not in st.session_state:
        st.session_state.controller = None
    if "info" not in st.session_state:
        st.session_state.info = None
    if "valve_state" not in st.session_state:
        st.session_state.valve_state = ["a", "b", "a", "b", "b", "a"]
    if "valve_state_last" not in st.session_state:
        st.session_state.valve_state_last = ["", "", "", "", "", ""]
    if "state_changed" not in st.session_state:
        st.session_state.state_changed = False

    # Draw the sidebar
    draw_sidebar()

    # Wait for the serial port to be connected
    if st.session_state.port_connected:
        st.session_state.valve_state_last = st.session_state.valve_state
        st.session_state.valve_state = st.session_state.controller.get_valves()

        # Run the Application
        app()

    with st.sidebar:
        st.markdown("----")
        st.markdown("PMEL/EDD")


if __name__ == "__main__":
    st.set_page_config(
        page_title="Streamlit Drawable Canvas Demo", page_icon=":pencil2:"
    )
    st.title("Flow Controller Dashboard")
    st.sidebar.subheader("Serial Port")
    main()
