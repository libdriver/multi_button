### 1. Chip

#### 1.1 Chip Info

Chip Name: STM32F407ZGT6.

Extern Oscillator: 8MHz.

GPIO Pin: ROW0/ROW1/ROW2/ROW3  PA0/PA1/PA2/PA3, COL0/COL1/COL2/COL3  PA4/PA5/PA6/PA7.

### 2. Development and Debugging

#### 2.1 Integrated Development Environment

LibDriver provides both Keil and IAR integrated development environment projects.

MDK is the Keil ARM project and your Keil version must be 5 or higher.Keil ARM project needs STMicroelectronics STM32F4 Series Device Family Pack and you can download from https://www.keil.com/dd2/stmicroelectronics/stm32f407zgtx.

EW is the IAR ARM project and your IAR version must be 9 or higher.

#### 2.2 Serial Port Parameter

Baud Rate: 115200.

Data Bits : 8.

Stop Bits: 1.

Parity: None.

Flow Control: None.

#### 2.3 Serial Port Assistant

We use '\n' to wrap lines.If your serial port assistant displays exceptions (e.g. the displayed content does not divide lines), please modify the configuration of your serial port assistant or replace one that supports '\n' parsing.

### 3. MULTI_BUTTON

#### 3.1 Command Instruction

1. Show multi_button chip and driver information.

    ```shell
    multi_button (-i | --information)  
    ```

2. Show multi_button help.

    ```shell
    multi_button (-h | --help)        
    ```

3. Show multi_button pin connections of the current board.

    ```shell
    multi_button (-p | --port)              
    ```

4. Run multi_button trigger test.

    ```shell
    multi_button (-t trigger | --test=trigger)
    ```

5. Run multi_button trigger function.

    ```shell
    multi_button (-e trigger | --example=trigger)  
    ```
    
#### 3.2 Command Example

```shell
multi_button -i

multi_button: chip is General MULTI_BUTTON.
multi_button: manufacturer is General.
multi_button: interface is GPIO.
multi_button: driver version is 1.0.
multi_button: min supply voltage is 1.8V.
multi_button: max supply voltage is 5.5V.
multi_button: max current is 1.00mA.
multi_button: max temperature is 125.0C.
multi_button: min temperature is -40.0C.
```

```shell
multi_button -p

multi_button: ROW0 connected to GPIOA PIN0.
multi_button: ROW1 connected to GPIOA PIN1.
multi_button: ROW2 connected to GPIOA PIN2.
multi_button: ROW3 connected to GPIOA PIN3.
multi_button: COL0 connected to GPIOA PIN4.
multi_button: COL1 connected to GPIOA PIN5.
multi_button: COL2 connected to GPIOA PIN6.
multi_button: COL3 connected to GPIOA PIN7.
```

```shell
multi_button -t trigger

multi_button: chip is General MULTI_BUTTON.
multi_button: manufacturer is General.
multi_button: interface is GPIO.
multi_button: driver version is 1.0.
multi_button: min supply voltage is 1.8V.
multi_button: max supply voltage is 5.5V.
multi_button: max current is 1.00mA.
multi_button: max temperature is 125.0C.
multi_button: min temperature is -40.0C.
multi_button: start trigger test.
multi_button: row 3 col 3 irq single click.
multi_button: row 3 col 3 irq double click.
multi_button: row 3 col 3 irq triple click.
multi_button: row 3 col 3 irq repeat click with 5 times.
multi_button: row 3 col 3 irq short press start.
multi_button: row 3 col 3 irq short press end.
multi_button: row 3 col 3 irq short press start.
multi_button: row 3 col 3 irq long press start.
multi_button: row 3 col 3 irq long press hold.
multi_button: row 3 col 3 irq long press end.
multi_button: finish trigger test.
```

```shell
multi_button -e trigger

multi_button: row 1 col 1 irq single click.
multi_button: row 1 col 1 irq double click.
multi_button: row 1 col 1 irq triple click.
multi_button: row 1 col 1 irq repeat click with 5 times.
multi_button: row 1 col 1 irq short press start.
multi_button: row 1 col 1 irq short press end.
multi_button: row 1 col 1 irq short press start.
multi_button: row 1 col 1 irq long press start.
multi_button: row 1 col 1 irq long press hold.
multi_button: row 1 col 1 irq long press end.
multi_button: trigger all.
```

```shell
multi_button -h

Usage:
  multi_button (-i | --information)
  multi_button (-h | --help)
  multi_button (-p | --port)
  multi_button (-t trigger | --test=trigger)
  multi_button (-e trigger | --example=trigger)

Options:
  -e <trigger>, --example=<trigger>    Run the driver example.
  -h, --help                           Show the help.
  -i, --information                    Show the chip information.
  -p, --port                           Display the pin connections of the current board.
  -t <trigger>, --test=<trigger>       Run the driver test.
```
