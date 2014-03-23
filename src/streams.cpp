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
static StreamBuf bufOut         (0);
static StreamBuf bufError       (E_ERROR);
static StreamBuf bufWarning     (E_WARNING);
static StreamBuf bufNotice      (E_NOTICE);
static StreamBuf bufDeprecated  (E_DEPRECATED);

/**
 *  Create the actual steams
 *  @var std::ostream
 */
std::ostream out        (&bufOut);
std::ostream error      (&bufError);
std::ostream warning    (&bufWarning);
std::ostream notice     (&bufNotice);
std::ostream deprecated (&bufDeprecated);
    
/**
 *  End namespace
 */
}

