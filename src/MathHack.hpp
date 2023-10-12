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

}
#endif