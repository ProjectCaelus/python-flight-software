import json
from enum import Enum, IntEnum, auto

class SensorType(str, Enum):
    THERMOCOUPLE = "thermocouple"
    PRESSURE = "pressure"
    LOAD = "load"


class SensorLocation(str, Enum):
    PT1 = "PT-1"
    PT2 = "PT-2"
    PT3 = "PT-3"
    PT4 = "PT-4"
    PT5 = "PT-5"
    PT6 = "PT-6"
    PT7 = "PT-7"
    PT8 = "PT-8"
    PTP = "PT-P"

class SolenoidState(IntEnum):
    OPEN = 1
    CLOSED = 0

class SensorStatus(IntEnum):
    SAFE = 3
    WARNING = 2
    CRITICAL = 1


class ValveType(str, Enum):
    SOLENOID = "solenoid"
    BALL = "ball"


class ValveLocation(str, Enum):
    PRESSURIZATION_VALVE = "pressurization_valve"
    VENT_VALVE = "vent_valve"
    REMOTE_DRAIN_VALVE = "remote_drain_valve"
    MAIN_PROPELLANT_VALVE = "main_propellant_valve"


class ActuationType(IntEnum):
    PULSE = 4
    OPEN_VENT = 3
    CLOSE_VENT = 2
    NONE = 1


class ValvePriority(IntEnum):
    NONE = 4
    LOW_PRIORITY = 3
    PI_PRIORITY = 2
    MAX_TELEMETRY_PRIORITY = 1
    ABORT_PRIORITY = 0


class Stage(str, Enum):
    WAITING = "waiting"
    PRESSURIZATION = "pressurization"
    AUTOSEQUENCE = "autosequence"
    POSTBURN = "postburn"
