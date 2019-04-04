#ifndef GENERAL_RESOURCES_HPP_
#define GENERAL_RESOURCES_HPP_

int     newly_data = 0;         // store newly weighted data, used in many .ino files         
int     n_apples   = 0;         // no. apples weighted

enum State {St_NULL, St_Calibrate, St_ConnectionCheck, St_ReadSensor, St_Wait, St_LCD_Button,
            St_Send,  St_SaveMem};
enum State   state, prev_state, p_prev_State;

enum SubState {st_calib_noload, st_calib, st_wait};
enum SubState sub_state;

#endif