#ifndef _MathHack
#define _MathHack

namespace MathHack{
    template<class T>
    inline constexpr const T& clamp(T& x, const T& min, const T& max){
       if(min > max){
        return clamp(x, max, min);
       }
    
       return (x < min) ? (x = min) : (x > max ? (x = max) : x); 
    }

    inline float Q_rsqrt(float number) {
       long i;
       float x2, y;
       const float threehalfs = 1.5F;

       x2 = number * 0.5F;
       y = number;
       i = *(long *)&y;
       i = 0x5f3759df - (i >> 1); 
       y = *(float *)&i;
       y = y * (threehalfs - (x2 * y * y)); 

       return y;
    }

    inline float calculateDistance(float x1, float y1, float x2, float y2) {
       float dx = x2 - x1;
       float dy = y2 - y1;
       float invSqrt = Q_rsqrt(dx * dx + dy * dy);
       return 1.0f / invSqrt;
    }
}
#endif