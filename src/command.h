/**
 * The MIT License (MIT)
 * 
 * Copyright (c) 2011-2014 sheeps.me
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * @package         proonga
 * @copyright       Copyright (c) 2011-2014 sheeps.me
 * @author          Yujiro Takahashi <yujiro3@gmail.com>
 * @filesource
 */

#ifndef HAVE_PROONGA_COMMAND_H
#define HAVE_PROONGA_COMMAND_H

extern int gqtpConnected;

/**
 * Proonga Command\‘¢‘Ì
 */
typedef struct prn_cmd_t {
    zval zcommand;
    zval zparam;
    grn_obj *command;
} prn_cmd;

int prngqtp_command(grn_ctx *ctx, zval *zcommand, const char *name TSRMLS_DC);
int prngqtp_command_set(grn_ctx *ctx, zval *zparam, const char *key, const char *value TSRMLS_DC);
int prngqtp_command_get(grn_ctx *ctx, zval *zparam, const char *key, zval *retval TSRMLS_DC);
int prngqtp_command_exec(grn_ctx *ctx, zval *zcommand, zval *zparam, zval *retval, int assoc TSRMLS_DC);
int prngqtp_command_unlink(zval *zcommand, zval *zparam TSRMLS_DC);

int proonga_command(grn_ctx *ctx, grn_obj **command, const char *name TSRMLS_DC);
int proonga_command_set(grn_ctx *ctx, grn_obj *command, const char *key, const char *value TSRMLS_DC);
int proonga_command_get(grn_ctx *ctx, grn_obj *command, const char *key, zval *retval TSRMLS_DC);
int proonga_command_exec(grn_ctx *ctx, grn_obj *command, zval *retval, int assoc TSRMLS_DC);

int prn_command(grn_ctx *ctx, prn_cmd *cmd, const char *name);

#define PRN_COMMAND(ctx, cmd, name) ((gqtpConnected == 1) ? \
    prngqtp_command(ctx, (zval *)&(cmd)->zcommand, name TSRMLS_CC) : \
    proonga_command(ctx, (prn_cmd *)&(cmd)->command, name TSRMLS_CC))

#define PRN_COMMAND_SET(ctx, cmd, key, val) ((gqtpConnected == 1) ? \
    prngqtp_command_set(ctx, (zval *)&(cmd)->zparam, key, val TSRMLS_CC) : \
    proonga_command_set(ctx, (cmd)->command, key, val TSRMLS_CC))

#define PRN_COMMAND_GET(ctx, cmd, key, val) ((gqtpConnected == 1) ? \
    prngqtp_command_get(ctx, (zval *)&(cmd)->zparam, key, val TSRMLS_CC) : \
    proonga_command_get(ctx, (cmd)->command, key, val TSRMLS_CC))

#define PRN_COMMAND_EXEC(ctx, cmd, retval, assoc) ((gqtpConnected == 1) ? \
    prngqtp_command_exec(ctx, (zval *)&(cmd)->zcommand, (zval *)&(cmd)->zparam, retval, assoc TSRMLS_CC) : \
    proonga_command_exec(ctx, (cmd)->command, retval, assoc TSRMLS_CC))

#define PRN_COMMAND_UNLINK(ctx, cmd) ((gqtpConnected == 1) ? \
    prngqtp_command_unlink((zval *)&(cmd)->zcommand, (zval *)&(cmd)->zparam TSRMLS_CC) : \
    grn_obj_unlink(ctx, (cmd)->command))


#endif
