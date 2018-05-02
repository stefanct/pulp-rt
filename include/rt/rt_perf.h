/*
 * Copyright (C) 2018 ETH Zurich, University of Bologna and GreenWaves Technologies
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __RT_RT_PERF_H__
#define __RT_RT_PERF_H__

/// @cond IMPLEM

#ifndef __riscv__

#define CSR_PCER_NB_EVENTS 0
#define CSR_PCER_CYCLES 0
#define CSR_PCER_INSTR 0
#define CSR_PCER_LD_STALL 0
#define CSR_PCER_JMP_STALL 0
#define CSR_PCER_IMISS 0
#define CSR_PCER_LD 0
#define CSR_PCER_ST 0
#define CSR_PCER_JUMP 0
#define CSR_PCER_BRANCH 0
#define CSR_PCER_TAKEN_BRANCH 0
#define CSR_PCER_RVC 0
#define CSR_PCER_LD_EXT 0
#define CSR_PCER_ST_EXT 0
#define CSR_PCER_LD_EXT_CYC 0
#define CSR_PCER_ST_EXT_CYC 0
#define CSR_PCER_TCDM_CONT 0
#define CSR_PCER_CSR_HAZARD 0
#define CSR_PCER_APU_TY_CONF 0
#define CSR_PCER_APU_CONT 0
#define CSR_PCER_APU_DEP 0
#define CSR_PCER_APU_WB 0

#endif

/// @endcond

/**        
 * @ingroup groupCluster       
 */

/**        
 * @defgroup Perf Performance counters
 *
 * This API gives access to the core performance counters. Each core has a few performance counters which can be configured 
 * to count one event out of several available. An event is a cycle, an instruction, a cache miss and so on. The number of counters
 * limits the number of events which can be monitored at the same time and depends on the architecture and the platform.
 *
 * In addition, this API uses a few other HW mechanisms useful for monitoring performance such as timers.
 *
 * To use the API, a structure of type rt_perf_t must be allocated and passed to most of the calls. This structure
 * contains the desired configuration and is used to save the values of the performance counters.
 * It can be used by one core or several (if concurrent accesses are protected).
 * The idea is that the hardware counters can be reset, started and stopped in order to get the event values for
 * a specific period and this can then be cumulated to the performance structure.
 */

/**        
 * @addtogroup Perf
 * @{        
 */

/** \enum rt_perf_event_e
 * \brief Performance event identifiers.
 *
 * This can be used to describe which performance event to monitor (cycles, cache miss, etc).
 */
typedef enum {
  RT_PERF_CYCLES        = CSR_PCER_NB_EVENTS,    /*!< Total number of cycles (also includes the cycles where the core is sleeping). Be careful that this event is using a timer shared within the cluster, so resetting, starting or stopping it on one core will impact other cores of the same cluster. */
  RT_PERF_ACTIVE_CYCLES = CSR_PCER_CYCLES,       /*!< Counts the number of cycles the core was active (not sleeping). */
  RT_PERF_INSTR         = CSR_PCER_INSTR,        /*!< Counts the number of instructions executed. */
  RT_PERF_LD_STALL      = CSR_PCER_LD_STALL,     /*!< Number of load data hazards. */  
  RT_PERF_JR_STALL      = CSR_PCER_JMP_STALL,    /*!< Number of jump register data hazards. */
  RT_PERF_IMISS         = CSR_PCER_IMISS,        /*!< Cycles waiting for instruction fetches, i.e. number of instructions wasted due to non-ideal caching. */
  RT_PERF_LD            = CSR_PCER_LD,           /*!< Number of data memory loads executed. Misaligned accesses are counted twice. */
  RT_PERF_ST            = CSR_PCER_ST,           /*!< Number of data memory stores executed. Misaligned accesses are counted twice. */
  RT_PERF_JUMP          = CSR_PCER_JUMP,         /*!< Number of unconditional jumps (j, jal, jr, jalr). */
  RT_PERF_BRANCH        = CSR_PCER_BRANCH,       /*!< Number of branches. Counts both taken and not taken branches. */
  RT_PERF_BTAKEN        = CSR_PCER_TAKEN_BRANCH, /*!< Number of taken branches. */
  RT_PERF_RVC           = CSR_PCER_RVC,          /*!< Number of compressed instructions executed. */
  RT_PERF_LD_EXT        = CSR_PCER_LD_EXT,       /*!< Number of memory loads to EXT executed. Misaligned accesses are counted twice. Every non-TCDM access is considered external (cluster only). */
  RT_PERF_ST_EXT        = CSR_PCER_ST_EXT,       /*!< Number of memory stores to EXT executed. Misaligned accesses are counted twice. Every non-TCDM access is considered external (cluster only). */
  RT_PERF_LD_EXT_CYC    = CSR_PCER_LD_EXT_CYC,   /*!< Cycles used for memory loads to EXT. Every non-TCDM access is considered external (cluster only). */
  RT_PERF_ST_EXT_CYC    = CSR_PCER_ST_EXT_CYC,   /*!< Cycles used for memory stores to EXT. Every non-TCDM access is considered external (cluster only). */
  RT_PERF_TCDM_CONT     = CSR_PCER_TCDM_CONT,    /*!< Cycles wasted due to TCDM/log-interconnect contention (cluster only). */
  RT_PERF_CSR_HAZARD    = CSR_PCER_CSR_HAZARD,   /*!< Cycles wasted due to CSR access. */
  RT_PERF_FP_TYPE       = CSR_PCER_APU_TY_CONF,  /*!< Cycles wasted due to different latencies of subsequent FP-operations (only available if a hardware floating-point unit is present). */
  RT_PERF_FP_CONT       = CSR_PCER_APU_CONT,     /*!< Cycles wasted due to contentions at the shared FPU  (only available if a hardware floating-point unit is present). */
  RT_PERF_FP_DEP        = CSR_PCER_APU_DEP,      /*!< Cycles wasted due to data hazards in subsequent FP instructions (only available if a hardware floating-point unit is present). */
  RT_PERF_FP_WB         = CSR_PCER_APU_WB,       /*!< Cycles wasted due to FP operations resulting in write-back contentions (only available if a hardware floating-point unit is present). */
} rt_perf_event_e;

/** \brief Initialize performance counters structure.
 *
 * This function initializes to 0 the value of all performance counters in the specified structure.
 * Note that this does not initialize the value of the hardware counters which can be done with rt_perf_reset.
 *
 * \param perf  A pointer to the performance structure.
 */
void rt_perf_init(rt_perf_t *perf);



/** \brief Configure performance counters.
 *
 * The set of events which can be activated at the same time depends on the architecture and the platform. On real chips (rather than with
 * the simulator), there is always only one counter. It is advisable to always use only one to be compatible with simulator and chip.
 *
 * At least RT_PERF_CYCLES and another event can be monitored at the same time as the first one is using the timer.
 *
 * \param perf  A pointer to the performance structure.
 * \param events A mask containing the events to activate. This is a bitfield, so events identifier must be used like 1 << RT_PERF_CYCLES.
 */
void rt_perf_conf(rt_perf_t *perf, unsigned events);



/** \brief Reset all hardware performance counters.
 *
 * All hardware performance counters are set to 0.
 * Note that this does not modify the value of the counters in the specified structure,
 * this must be done by calling rt_perf_init.
 *
 * \param perf  A pointer to the performance structure.
 */
static inline void rt_perf_reset(rt_perf_t *perf);



/** \brief Start monitoring configured events.
 *
 * This function is useful for finely controlling the point where performance events start being monitored.
 * The counter retains its value between stop and start so it is possible to easily sum events for several
 * portions of code.
 *
 * \param perf  A pointer to the performance structure.
 */
static inline void rt_perf_start(rt_perf_t *perf);



/** \brief Stop monitoring configured events.
 *
 * \param perf  A pointer to the performance structure.
 */
static inline void rt_perf_stop(rt_perf_t *perf);



/** \brief Save performance counters values.
 *
 * This loads the performance counters values and adds them inside the performance structure. 
 * This is useful when reconfiguring the events in order to aggregate all the events together into the performance structure.
 * Note that the values are cumulated inside the performance structure so that several portion
 * of code can be cumulated together. Don't forget to reset the HW counters with rt_perf_reset
 * or the the counters in the structure with rt_perf_init if you don't want to cumulate
 * the values.
 *
 * \param perf  A pointer to the performance structure.
 */
void rt_perf_save(rt_perf_t *perf);



/** \brief Get performance counters values.
 *
 * This returns the saved values, thus rt_perf_save must be called before at least once.
 *
 * \param perf  A pointer to the performance structure.
 * \param id    The performance event identifier to get.
 * \return      The performance counter value.
 */
static inline unsigned int rt_perf_get(rt_perf_t *perf, int id);



/** \brief Get performance counters values average.
 *
 * This returns the saved values, thus rt_perf_save must be called before at least once.
 * This will compute an average of the specified event between all the specified
 * performance structures.
 *
 * \param perf  A pointer to an array of performance structures.
 * \param id    The performance event identifier to get.
 * \param nb_cores The number of performance structures on which the average must be done.
 * \return      The performance counter value.
 */
unsigned int rt_perf_get_average(rt_perf_t *perf, int id, int nb_cores);



/** \brief Read a performance counter
 *
 * This does a direct read to the specified performance counter. Calling this functions is useful for getting
 * the performance counter with very low overhead (just few instructions).
 * 
 * \param id    The performance event identifier to read.
 * \return      The performance counter value.
 */
static inline unsigned int rt_perf_read(int id);

//!@}



/// @cond IMPLEM

#if defined(TIMER_VERSION) && TIMER_VERSION >= 2

static inline void rt_perf_cl_reset(rt_perf_t *perf)
{
  hal_timer_reset(hal_timer_addr(0, 0));
  cpu_perf_setall(0);
}

static inline void rt_perf_fc_reset(rt_perf_t *perf)
{
#ifdef ARCHI_HAS_FC
  hal_timer_reset(hal_timer_fc_addr(0, 0));
  cpu_perf_setall(0);
#endif
}

static inline void rt_perf_reset(rt_perf_t *perf)
{
  if (hal_is_fc())
    rt_perf_fc_reset(perf);
  else
    rt_perf_cl_reset(perf);
}

static inline void rt_perf_cl_start(rt_perf_t *perf)
{
  hal_timer_start(hal_timer_addr(0, 0));
  cpu_perf_conf(PCMR_ACTIVE | PCMR_SATURATE);
}

static inline void rt_perf_fc_start(rt_perf_t *perf)
{
#ifdef ARCHI_HAS_FC
  hal_timer_start(hal_timer_fc_addr(0, 0));
  cpu_perf_conf(PCMR_ACTIVE | PCMR_SATURATE);
#endif
}

static inline void rt_perf_start(rt_perf_t *perf)
{
  if (hal_is_fc())
    rt_perf_fc_start(perf);
  else
    rt_perf_cl_start(perf);
}

static inline void rt_perf_cl_stop(rt_perf_t *perf)
{
  hal_timer_conf(hal_timer_addr(0, 0), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  cpu_perf_conf(0);
}

static inline void rt_perf_fc_stop(rt_perf_t *perf)
{
#ifdef ARCHI_HAS_FC
  hal_timer_conf(hal_timer_fc_addr(0, 0), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  cpu_perf_conf(0);
#endif
}

static inline void rt_perf_stop(rt_perf_t *perf)
{
  if (hal_is_fc())
    rt_perf_fc_stop(perf);
  else
    rt_perf_cl_stop(perf);
}

static inline unsigned int rt_perf_get(rt_perf_t *perf, int id)
{
  return perf->values[id];
}

static inline unsigned int rt_perf_cl_read(int event)
{
  if (event == RT_PERF_CYCLES)
  {
    return hal_timer_count_get(hal_timer_addr(0, 0));
  }
  else
  {
    return cpu_perf_get(event);
  }
}

static inline unsigned int rt_perf_fc_read(int event)
{
#ifdef ARCHI_HAS_FC
  if (event == RT_PERF_CYCLES)
  {
    return hal_timer_count_get(hal_timer_fc_addr(0, 0));
  }
  else
  {
    return cpu_perf_get(event);
  }
#else
  return 0;
#endif
}

static inline unsigned int rt_perf_read(int event)
{
  if (hal_is_fc())
    return rt_perf_fc_read(event);
  else
    return rt_perf_cl_read(event);
}

#endif

/// @endcond

#endif
