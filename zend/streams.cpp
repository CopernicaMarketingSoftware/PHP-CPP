/**
 *  Streams.cpp
 *
 *  Implementation of the streams
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */
#include "includes.h"

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Some static buffers for writing data
 *  @var StreamBuf
 */
static thread_local StreamBuf bufOut        (0);
static thread_local StreamBuf bufError      (E_ERROR);
static thread_local StreamBuf bufWarning    (E_WARNING);
static thread_local StreamBuf bufNotice     (E_NOTICE);
static thread_local StreamBuf bufDeprecated (E_DEPRECATED);

/**
 *  Create the actual steams
 *  @var std::ostream
 */
thread_local std::ostream out               (&bufOut);
thread_local std::ostream error             (&bufError);
thread_local std::ostream warning           (&bufWarning);
thread_local std::ostream notice            (&bufNotice);
thread_local std::ostream deprecated        (&bufDeprecated);

/**
 *  End namespace
 */
}

