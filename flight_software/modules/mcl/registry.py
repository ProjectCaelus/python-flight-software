from modules.mcl.mode import Mode
from modules.mcl.status import Status
from modules.mcl.errors import SetError
from modules.mcl.logging import Packet
import time

class Registry:


    # TODO: Add IMU data, Pressure data, Load cell data, and Valve data to SFR
    def __init__(self):
        self.values = {
            "thermocouple": None,
            "thermocouple_status": Status.WORKING,
            "pressure_gas": None,
            "load_cell_h20": None,
            "telemetry_queue": [],
            "mode": None
        }
        self.types = {
            "thermocouple": float,
            "thermocouple_status": Status,
            "pressure_gas": float,
            "load_cell_h20": float,
            "telemetry_queue": list,
            "mode": Mode
        }
        self.times = {i: None for i in self.values}

    def put(self, key, value) -> SetError:
        if key not in self.values:
            return SetError.PUT_ERROR
        print(value)
        print(type(value))
        if not isinstance(value, self.types[key]):
            return SetError.PUT_ERROR
        self.values[key] = value
        self.times[key] = time.time()
        return SetError.NONE

    def get(self, key):
        if key not in self.values:
            return Error.ERROR
        return self.values[key]
