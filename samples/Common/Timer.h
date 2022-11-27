#ifndef __TIMER_H

#define __TIMER_H

class Timer {
protected:
    int using_mm;
    double frequency;
public:
    Timer();
    double GetTime() const;
    double GetFrequency() const
        { return frequency; }
};

extern Timer SystemTimer;

inline double ms2ticks(double ms) // ������� ����������� � ���� �������
	{ return ms*SystemTimer.GetFrequency()/1000.0; }

inline double ticks2ms(double ticks) // ������� ����� ������� � ������������
	{ return ticks*1000.0/SystemTimer.GetFrequency(); }

#endif // __TIMER_H

