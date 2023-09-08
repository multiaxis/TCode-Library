#include "TCodeAxis.h"


TCodeAxis::TCodeAxis(const char* name)
{
    currentState.ramp_type = TCode_Axis_Ramp_Type::Linear;
    currentState.startTime = 0;
    currentState.endTime = 0;
    currentState.startValue = TCODE_DEFAULT_AXIS_RETURN_VALUE;
    currentState.endValue = TCODE_DEFAULT_AXIS_RETURN_VALUE;
    axisName = name;
    last_command_time = 0;
    minInterval = TCODE_MIN_AXIS_SMOOTH_INTERVAL;
}


unsigned long TCodeAxis::getDeltaTime(int start_position, int target_value, TCode_Axis_Extention_Type extention_type, long extention_value,TCode_Axis_Ramp_Type ramp_type)
{
    unsigned long delta_time = 0;

    switch (extention_type)
    {
        case TCode_Axis_Extention_Type::Speed:
        {
            delta_time  = abs(target_value - start_position);
            delta_time  *= 100;
            if(extention_value > 0)
            {
                delta_time  /= extention_value;
            }
        }
        break;
        case TCode_Axis_Extention_Type::Time:
        default:
        {
            if(extention_value > 0)
                delta_time = extention_value;
            else 
                delta_time = minInterval;
        }
    }
}

/**
 * @brief sets the axis' dynamic parameters
 * @param target_value the target value for this axis
 * @param extention_type the extention type for the axis e.g. Time,Speed
 * @param extention_value the extention value
 * @param ramp_type the ramp type
 */
void TCodeAxis::set(int target_value, TCode_Axis_Extention_Type extention_type = TCode_Axis_Extention_Type::Time, long extention_value = 0,TCode_Axis_Ramp_Type ramp_type = TCode_Axis_Ramp_Type::Linear)
{
    unsigned long t = getLastMillis(); // This is for adding this state
    target_value = constrain(target_value, 0, TCODE_MAX_AXIS);
    extention_value = constrain(extention_value, 0, TCODE_MAX_AXIS_MAGNITUDE);

    int last_target = getLastTargetPosition();
    unsigned long delta_time = getDeltaTime(last_target,target_value,extention_type,extention_value,ramp_type);

    TCode_Axis_State new_state;
    new_state.startTime = t;
    new_state.endTime = t + delta_time;
    new_state.startValue = last_target;
    new_state.endValue = target_value;
    new_state.ramp_type = ramp_type;
    dataQueue.push(new_state);
    
    int lastInterval = millis() - last_command_time;
    int difference = abs(lastInterval - minInterval);
    if ((lastInterval > minInterval) && (minInterval < TCODE_MAX_AXIS_SMOOTH_INTERVAL))
    {
        minInterval += difference/2;
    }
    else if ((lastInterval < minInterval) && (minInterval > TCODE_MIN_AXIS_SMOOTH_INTERVAL))
    {
        minInterval -= difference/2;
    }
    minInterval = constrain(minInterval,TCODE_MIN_AXIS_SMOOTH_INTERVAL,TCODE_MAX_AXIS_SMOOTH_INTERVAL);
    last_command_time = millis();
}

unsigned long TCodeAxis::getLastMillis()
{
    unsigned long currentTime = millis();
    if(dataQueue.isEmpty())
    {
        if(currentTime > currentState.endTime)
            return currentTime;
        else
            return currentState.endTime;
    }

    TCode_Axis_State backState;
    dataQueue.peekBack(backState);
    return backState.endTime;
}

int TCodeAxis::getLastTargetPosition()
{
    unsigned long current_position = 0;
    if(dataQueue.isEmpty())
    {
        return currentState.endValue;
    }

    TCode_Axis_State backState;
    dataQueue.peekBack(backState);
    return backState.endValue;
}

/**
 * @brief gets the current position of the axis
 * @returns current position of this axis as an int
 */
int TCodeAxis::getPosition()
{
    bool isInRange = false;
    int value = getPositionMapped(currentState,isInRange);
    if(isInRange)
    {
        return value;
    }
    
    if(dataQueue.isEmpty())
    {
        return value;
    }

    while(!isInRange) {
        dataQueue.pop(currentState);
        value = getPositionMapped(currentState,isInRange);
        if(dataQueue.isEmpty())
            break;
    }

    return value;
}

int TCodeAxis::getPositionMapped(TCode_Axis_State &state, bool& inRange)
{
    int x; // This is the current axis position, 0-9999
    unsigned long t = millis();
    inRange = true;
    if (t >= state.endTime)
    {
        inRange = false;
        return state.endValue;
    }

    if(t < state.startTime)
    {
        inRange = false;
        return state.startValue;
    }


    switch (state.ramp_type)
    {
    case TCode_Axis_Ramp_Type::Linear:
        x = mapQ16(t, state.startTime, state.endTime, state.startValue, state.endValue);
        break;
    case TCode_Axis_Ramp_Type::EaseIn:
        x = mapEaseIn(t, state.startTime, state.endTime, state.startValue, state.endValue);
        break;
    case TCode_Axis_Ramp_Type::EaseOut:
        x = mapEaseOut(t, state.startTime, state.endTime, state.startValue, state.endValue);
        break;
    case TCode_Axis_Ramp_Type::EaseInOut:
        x = mapEaseInOut(t, state.startTime, state.endTime, state.startValue, state.endValue);
        break;
    default:
        x = mapQ16(t, state.startTime, state.endTime, state.startValue, state.endValue);
    }


    if(x > TCODE_MAX_AXIS)
        x = TCODE_MAX_AXIS;
    if(x < 0)
        x = 0;

    return x;
}

/**
 * @brief stops axis movement at its current position
 */
void TCodeAxis::stop()
{
    unsigned long t = millis(); // This is the time now
    bool inRange;
    currentState.endValue = getPositionMapped(currentState,inRange);
    currentState.startTime = t;
    currentState.endTime = t;
    dataQueue.clear();
}

/**
 * @brief stops axis movement at its current position
 * @returns returns true if the axis has changed position since last check
 */
bool TCodeAxis::changed()
{
    if (lastPosition != getPosition())
    {
        lastPosition = getPosition();
        return true;
    }
    return false;
}

/**
 * @brief gets the name of the axis
 * @returns returns the string representation of the name stored
 */
const char* TCodeAxis::getName()
{
    return axisName;
}
