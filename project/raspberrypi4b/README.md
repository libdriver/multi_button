### 1. Board

#### 1.1 Board Info

Board Name: Raspberry Pi 4B.

GPIO Pin: ROW0/ROW1/ROW2/ROW3  GPIO17/GPIO27/GPIO22/GPIO5, COL0/COL1/COL2/COL3  GPIO6/GPIO13/GPIO19/GPIO26.

### 2. Install

#### 2.1 Dependencies

Install the necessary dependencies.

```shell
sudo apt-get install libgpiod-dev pkg-config cmake -y
```

#### 2.2 Makefile

Build the project.

```shell
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

#### 2.3 CMake

Build the project.

```shell
mkdir build && cd build 
cmake .. 
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

Test the project and this is optional.

```shell
make test
```

Find the compiled library in CMake. 

```cmake
find_package(multi_button REQUIRED)
```

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
./multi_button -i

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
./multi_button -p

multi_button: ROW0 connected to GPIO17(BCM).
multi_button: ROW1 connected to GPIO27(BCM).
multi_button: ROW2 connected to GPIO22(BCM).
multi_button: ROW3 connected to GPIO5(BCM).
multi_button: COL0 connected to GPIO6(BCM).
multi_button: COL1 connected to GPIO13(BCM).
multi_button: COL2 connected to GPIO19(BCM).
multi_button: COL3 connected to GPIO26(BCM).
```

```shell
./multi_button -t trigger

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
./multi_button -e trigger

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
./multi_button -h

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
