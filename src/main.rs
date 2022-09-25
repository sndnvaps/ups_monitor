
extern crate linux_embedded_hal as hal;

extern crate ina219_rs as ina219;

use hal::I2cdev;
use ina219::physic;

use std::time::Duration;
use std::thread::sleep;

pub mod ctrlc_utils;

use ina219::ina219::{INA219,Opts};

fn main() {

    let device = I2cdev::new("/dev/i2c-1").unwrap();
    let opt = Opts::new(0x42,100 * physic::MilliOhm,1 * physic::Ampere);
    //let opt = Opts::default();
    let mut ina = INA219::new(device,opt);
    ina.init().unwrap();

    let ctrlc = ctrlc_utils::CtrlcHandler::init();
    println!("Press Ctrl+C to shutdown it!");
   
    loop {
            let pm = ina.sense().unwrap();
            println!("{:?}",pm);
            sleep(Duration::from_secs(3));
            let is_ctrlc = ctrlc.poll_ctrlc();
            if is_ctrlc {
                println!("Get quit signal, just bye!");
                break;
            }
    }




    /* 
    let voltage = ina.voltage().unwrap();
    println!("bus voltage: {:?}",voltage);

    let voltage_raw = ina.voltage_raw().unwrap();
    println!("bus voltage_raw: {:?}",voltage_raw);

    let shunt = ina.shunt_voltage().unwrap();
    println!("shunt voltage: {:?}",shunt);

    let current = ina.current().unwrap();
    println!("current: {:?}",current);

    let current_raw = ina.current_raw().unwrap();
    println!("current_raw: {:?}",current_raw);

    let power = ina.power().unwrap();
    println!("power: {:?}",power);
    */

}
