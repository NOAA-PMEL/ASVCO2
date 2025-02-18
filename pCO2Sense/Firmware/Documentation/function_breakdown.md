
- [ ] PCO2
  - [ ] Comments
  * Global Functions
    - [ ] Initialize
    - [ ] Run


  * Static Functions
    - [ ] State: Config
    - [x] State: Zero
    - [x] State: Span
    - [ ] State: Span2
    - [x] State: Equil
    - [x] State: Air
    - [x] State: Rest
    - [x] Sub-State: Calibrate
    - [x] Sub-State: Sample


- [x] flow control
    - [ ] Comments
  * Global Function
    - [x] Initialize
    - [x] Wakeup
    - [x] Sleep
    - [x] Set Mode
    - [x] Get Mode
  
  * Static Function
    - [x] Startup
    - [x] Power On
    - [x] Power Off


		
- [ ] RH
  - [ ] Comments
    - [ ] Initialize
    - [ ] Read RH, Temp (and Pressure)
    - [ ] Sleep
		
- [x] O2
		- [ ] Comments
    - [x] Initialize
    - [x] Read O2
    - [ ] Sleep
		
- [ ] Statistics
		- [ ] Comments
    - [ ] Initialize
    - [ ] Clear Data (Float)
    - [ ] Add Data (Float)
    - [ ] Calculate Data (Float)
    - [ ] Clear Data (uint32)
    - [ ] Add Data (uint32)
    - [ ] Calculate Data (uint32)
    - [ ] Clear Data (uint16)
    - [ ] Add Data (uint16)
    - [ ] Calculate Data (uint16)
    - [ ] Clear Data (uint8)
    - [ ] Add Data (uint8)
    - [ ] Calculate Data (uint8)
    - [ ] Clear Data (int32)
    - [ ] Add Data (int32)
    - [ ] Calculate Data (int32)
    - [ ] Clear Data (int16)
    - [ ] Add Data (int16)
    - [ ] Calculate Data (int16)
    - [ ] Clear Data (int8)
    - [ ] Add Data (int8)
    - [ ] Calculate Data (int8)


- [x] RTC
    - [x] Comments
  * GLOBAL FUNCTINOS
    - [x] Initialize
    - [x] Set Time
    - [x] Get Time
    - [x] Get Timestamp
    - [x] Set Alarm
    - [x] Set Alarm With Repeat
    - [x] Set Alarm Next
    - [x] Clear Alarm
    - [x] Set Timer
    - [x] Offset Calculation
    - [x] Crystal Cal Output Wave
    - [x] Crystal Cal Clear Wave

  * STATIC FUNCTIONS
    - [x] Write Register
    - [x] Burst Write
    - [x] Read Register
    - [x] Burst Read
    - [x] Clear Register
    - [x] Set Register
    - [x] Oscillator select
    - [x] Configure Square Wave
    - [x] get time
    - [x] set alarm
    - [x] bcd2dec
    - [x] dec2bcd
    - [x] Validate Year
		- [x] Validate Month
    - [x] Validate Day
    - [x] Validate Hour
    - [x] Validate Minute
    - [x] Validate Second

		
- [x] MAX3109
		- [x] Comments
    - [x] Initialize
    - [x] Read Buffer
    - [x] Write Buffer
    - [x] Set Read Interrupt
    - [x] Set Write Interrupt
    - [x] Sleep
		
- [ ] SDL
		- [ ] Comments
    - [ ] Initialize
    - [ ] Write Data
    - [ ] Sleep
		
- [ ] Console
		- [ ] Comments
    - [ ] Set Passthrough to Licor
    - [ ] Set Passthrough to FlowController
    - [ ] Read System Settings
    - [ ] Set Time
    - [ ] Read Time
    - [ ] Set Serial #
    - [ ] Read Serial #
    - [ ] Set Debug Level
    - [ ] Read Debug Level
    - [ ] Set Error Level
    - [ ] Read Error Level
    - [ ] Set Log Level
    - [ ] Read Log Level
    - [ ] TELOS Command Set

- [x] ConsoleIO
  - [x] Comments
  - [x] Initialize
  - [X] Receive
  - [ ] Close (@todo Not sure if this is needed)
		
- [ ] Command
		- [ ] Comments
    - [ ] Initialize
    - [ ] Receive
    - [ ] Close (@todo Not sure if this is needed)
		
- [x] Scheduler
		- [x] Comments
    - [x] Create Task
    - [x] Execute Task
    - [x] Delete Task