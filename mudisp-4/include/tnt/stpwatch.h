// Template Numerical Toolkit (TNT) for Linear Algebra 
//
// R. Pozo
// Applied and Computational Mathematics Division
// National Institute of Standards and Technology


#ifndef STPWATCH_H
#define STPWATCH_H

// for clock() and CLOCKS_PER_SEC
#include <ctime>

namespace TNT
{

/*  Simple stopwatch object:

        void    start()     : start timing
        double  stop()      : stop timing
        void    reset()     : set elapsed time to 0.0
        double  read()      : read elapsed time (in seconds)

*/

/** @fn inline double seconds(void)
 * @brief Cronometro 
 * @return  number of clocks for second
 * */
inline double seconds(void)
{
    static const double secs_per_tick = 1.0 / CLOCKS_PER_SEC;
    return ( (double) clock() ) * secs_per_tick;
}

/** @class stopwatch stpwatch.h <tnt/stpwathc.h>
 * @brief Time Operation
 * */
class stopwatch {
    private:
		/**@var int running
		 * @brief number of time running
		 * */
        int running;
        /** @var double last_time
         * @brief last rilevate time
         * */
        double last_time;
        /** var double total
         * @brief total elapse time 
         * */
        double total;

    public:
    /** @fn stopwatch() : running(0), last_time(0.0), total(0.0)
     * @brief class constructor
     * */
        stopwatch() : running(0), last_time(0.0), total(0.0) {}
     /** @fn void reset() 
      * @brief Time reset
      * */   
        void reset() { running = 0; last_time = 0.0; total=0.0; }
      /** @fn void start() 
       * @brief Time Start
       * */
        void start() { if (!running) { last_time = seconds(); running = 1;}}
        
        /** @fn double stop()
         * @brief Time count stop
         * @return Elapse Time
         * */
        double stop()  { if (running) 
                            {
                                total += seconds() - last_time; 
                                running = 0;
                             }
                          return total; 
                        }
        /** @fn double read()
         * @brief Read elapse time
         * @return elapse time
         * */               
        double read()   {  if (running) 
                            {
                                total+= seconds() - last_time;
                                last_time = seconds();
                            }
                           return total;
                        }       
                            
};

} // namespace TNT

#endif
    

            
