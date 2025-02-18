from src.FlowController import FlowControl, MODES, InfoTypes
from typing import Optional
import typer
import json

# Load Typer
main = typer.Typer()


@main.command()
def setup(port: str):
    """Run this command FIRST"""
    with open(".env", "w") as f:
        json.dump({"PORT": port}, f)


def create_flow_control():
    """Creates a flow control object"""
    with open(".env", "r") as f:
        data = f.read()
    obj = json.loads(data)
    if not obj["PORT"]:
        typer.secho(
            "You need to specify a port to run the flow control",
            fg=typer.colors.RED,
            err=True,
        )
    return FlowControl(obj["PORT"])


@main.command()
def info():
    """
    Print information about the Flow Controller.
    """
    fc = create_flow_control()
    with fc:
        typer.echo(f"FW Version:\r\t\t{fc.get_info(InfoTypes.VERSION)}")
        typer.echo(f"Pulse: \r\t\t{fc.get_info(InfoTypes.PULSE)}")
        typer.echo(f"Current: \r\t\t{fc.get_info(InfoTypes.CURRENT)}")
        typer.echo(f"Count: \r\t\t{fc.get_info(InfoTypes.COUNT)}")
        typer.echo(f"Mode: \r\t\t{fc.get_info(InfoTypes.MODE)}")


@main.command()
def valve(valve: int, direction: str):
    """
    Set a valve on the Flow Controller.

    Args:
        valve: valve number
        direction: 'ac' or 'bc'
    """
    fc = create_flow_control()
    with fc:
        try:
            fc.set_valve(valve, direction.lower())
            typer.echo(f"Valve {valve} Set to {direction} ")
        except ValueError:
            typer.echo(f"Invalid Valve/Direction combo {valve}/{direction}")
            typer.echo(f"Valid Valves values are 1 - 6")
            typer.echo(f"Valid Directions are 'ac' or 'bc'")


@main.command()
def valves(valves: str):
    """Set all six valves on the Flow Controller
    Args:
        valves[str]: String of six valves directions (A or B) to set

    \nExample (Set the valves to [AC, BC, AC, BC, AC, BC]):\n\n
        > python main.py valves ABABAB

    """
    if len(valves) < 6:
        raise ValueError("Input must have six valve states")

    valves = [*valves]
    for idx, v in enumerate(valves):
        valve(idx + 1, v + "c")

    state()


@main.command()
def state():
    """Get the current valve state"""
    fc = create_flow_control()
    with fc:
        valves = fc.get_valves()
        print(f"Current State: {''.join(valves)}")


@main.command()
def mode(mode: Optional[str] = typer.Argument(None)):
    """Set Flow Controller mode

    Args:
        mode (str): Set mode to specified
    """
    fc = create_flow_control()
    with fc:
        if mode:

            try:
                fc.set_mode(mode)
                typer.echo(f"Mode Set to {mode} ")
            except ValueError:
                typer.echo(f"Invalid Mode: {mode}")
                typer.echo(f"Valid modes are: ")
                values = ""
                temp = "\t"
                for idx, mode in enumerate(MODES):
                    temp += f"{mode}, "
                    if idx > 0 and idx % 8 == 0:
                        values += temp + "\n"
                        temp = "\t"
                typer.echo(values[:-3])
        typer.echo(f"Mode: \r\t\t{fc.get_info(InfoTypes.MODE)}")


if __name__ == "__main__":
    main()
