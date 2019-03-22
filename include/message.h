/**
 *  Message.h
 *
 *  In this file an enumeration type is defined with all error-message flags.
 *
 *  @author Toon Schoenmakers <toon.schoenmakers@copernica.com>
 *  @copyright 2015 - 2019 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Supported types of errors, this is mostly a copy from Zend/zend_errors.h
 */
enum class Message : int {
    Error                =   (1 << 0L),
    Warning              =   (1 << 1L),
    Parse                =   (1 << 2L),
    Notice               =   (1 << 3L),
    CoreError            =   (1 << 4L),
    CoreWarning          =   (1 << 5L),
    CompileError         =   (1 << 6L),
    CompileWarning       =   (1 << 7L),
    UserError            =   (1 << 8L),
    UserWarning          =   (1 << 9L),
    UserNotice           =   (1 << 10L),
    Strict               =   (1 << 11L),
    RecoverableError     =   (1 << 12L),
    Deprecated           =   (1 << 13L),
    UserDeprecated       =   (1 << 14L),

    Core                 =   (CoreError | CoreWarning),
    All                  =   (Error | Warning | Parse | Notice | CoreError | CoreWarning | CompileError | CompileWarning | UserError | UserWarning | UserNotice | RecoverableError | Deprecated | UserDeprecated )
};

/**
 *  End of namespace
 */
}
