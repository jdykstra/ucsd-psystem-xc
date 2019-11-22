//
// UCSD p-System cross compiler
// Copyright (C) 2010, 2012 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or (at
// you option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program. If not, see <http://www.gnu.org/licenses/>
//

#include <ucsdpsys_assemble/expression/special/a.h>
#include <ucsdpsys_assemble/expression/special/x.h>
#include <ucsdpsys_assemble/expression/special/y.h>
#include <ucsdpsys_assemble/opcode/6502/adc.h>
#include <ucsdpsys_assemble/opcode/6502/and.h>
#include <ucsdpsys_assemble/opcode/6502/asl.h>
#include <ucsdpsys_assemble/opcode/6502/bbr.h>
#include <ucsdpsys_assemble/opcode/6502/bbs.h>
#include <ucsdpsys_assemble/opcode/6502/bcc.h>
#include <ucsdpsys_assemble/opcode/6502/bcs.h>
#include <ucsdpsys_assemble/opcode/6502/beq.h>
#include <ucsdpsys_assemble/opcode/6502/bit.h>
#include <ucsdpsys_assemble/opcode/6502/bmi.h>
#include <ucsdpsys_assemble/opcode/6502/bne.h>
#include <ucsdpsys_assemble/opcode/6502/bpl.h>
#include <ucsdpsys_assemble/opcode/6502/bra.h>
#include <ucsdpsys_assemble/opcode/6502/brk.h>
#include <ucsdpsys_assemble/opcode/6502/bvc.h>
#include <ucsdpsys_assemble/opcode/6502/bvs.h>
#include <ucsdpsys_assemble/opcode/6502/clc.h>
#include <ucsdpsys_assemble/opcode/6502/cld.h>
#include <ucsdpsys_assemble/opcode/6502/cli.h>
#include <ucsdpsys_assemble/opcode/6502/clv.h>
#include <ucsdpsys_assemble/opcode/6502/cmp.h>
#include <ucsdpsys_assemble/opcode/6502/cpx.h>
#include <ucsdpsys_assemble/opcode/6502/cpy.h>
#include <ucsdpsys_assemble/opcode/6502/dea.h>
#include <ucsdpsys_assemble/opcode/6502/dec.h>
#include <ucsdpsys_assemble/opcode/6502/dex.h>
#include <ucsdpsys_assemble/opcode/6502/dey.h>
#include <ucsdpsys_assemble/opcode/6502/eor.h>
#include <ucsdpsys_assemble/opcode/6502/ina.h>
#include <ucsdpsys_assemble/opcode/6502/inc.h>
#include <ucsdpsys_assemble/opcode/6502/inx.h>
#include <ucsdpsys_assemble/opcode/6502/iny.h>
#include <ucsdpsys_assemble/opcode/6502/jmp.h>
#include <ucsdpsys_assemble/opcode/6502/jsr.h>
#include <ucsdpsys_assemble/opcode/6502/lda.h>
#include <ucsdpsys_assemble/opcode/6502/ldx.h>
#include <ucsdpsys_assemble/opcode/6502/ldy.h>
#include <ucsdpsys_assemble/opcode/6502/lsr.h>
#include <ucsdpsys_assemble/opcode/6502/nop.h>
#include <ucsdpsys_assemble/opcode/6502/ora.h>
#include <ucsdpsys_assemble/opcode/6502/pha.h>
#include <ucsdpsys_assemble/opcode/6502/php.h>
#include <ucsdpsys_assemble/opcode/6502/phx.h>
#include <ucsdpsys_assemble/opcode/6502/phy.h>
#include <ucsdpsys_assemble/opcode/6502/pla.h>
#include <ucsdpsys_assemble/opcode/6502/plp.h>
#include <ucsdpsys_assemble/opcode/6502/plx.h>
#include <ucsdpsys_assemble/opcode/6502/ply.h>
#include <ucsdpsys_assemble/opcode/6502/rmb.h>
#include <ucsdpsys_assemble/opcode/6502/rol.h>
#include <ucsdpsys_assemble/opcode/6502/ror.h>
#include <ucsdpsys_assemble/opcode/6502/rti.h>
#include <ucsdpsys_assemble/opcode/6502/rts.h>
#include <ucsdpsys_assemble/opcode/6502/sbc.h>
#include <ucsdpsys_assemble/opcode/6502/sec.h>
#include <ucsdpsys_assemble/opcode/6502/sed.h>
#include <ucsdpsys_assemble/opcode/6502/sei.h>
#include <ucsdpsys_assemble/opcode/6502/smb.h>
#include <ucsdpsys_assemble/opcode/6502/sta.h>
#include <ucsdpsys_assemble/opcode/6502/stp.h>
#include <ucsdpsys_assemble/opcode/6502/stx.h>
#include <ucsdpsys_assemble/opcode/6502/sty.h>
#include <ucsdpsys_assemble/opcode/6502/stz.h>
#include <ucsdpsys_assemble/opcode/6502/tax.h>
#include <ucsdpsys_assemble/opcode/6502/tay.h>
#include <ucsdpsys_assemble/opcode/6502/trb.h>
#include <ucsdpsys_assemble/opcode/6502/tsb.h>
#include <ucsdpsys_assemble/opcode/6502/tsx.h>
#include <ucsdpsys_assemble/opcode/6502/txa.h>
#include <ucsdpsys_assemble/opcode/6502/txs.h>
#include <ucsdpsys_assemble/opcode/6502/tya.h>
#include <ucsdpsys_assemble/opcode/6502/wai.h>
#include <ucsdpsys_assemble/personality/6502.h>


personality_6502::~personality_6502()
{
}


personality_6502::personality_6502(assembler *a_context, chip_model_t model) :
    context(*a_context)
{
    register_opcode(opcode_6502_adc::create(a_context, model));
    register_opcode(opcode_6502_and::create(a_context, model));
    register_opcode(opcode_6502_asl::create(a_context));
    register_opcode(opcode_6502_bbr::create(a_context, model));
    register_opcode(opcode_6502_bbs::create(a_context, model));
    register_opcode(opcode_6502_bcc::create(a_context));
    register_opcode(opcode_6502_bcs::create(a_context));
    register_opcode(opcode_6502_beq::create(a_context));
    register_opcode(opcode_6502_bit::create(a_context, model));
    register_opcode(opcode_6502_bmi::create(a_context));
    register_opcode(opcode_6502_bne::create(a_context));
    register_opcode(opcode_6502_bpl::create(a_context));
    register_opcode(opcode_6502_bra::create(a_context, model));
    register_opcode(opcode_6502_brk::create(a_context));
    register_opcode(opcode_6502_bvc::create(a_context));
    register_opcode(opcode_6502_bvs::create(a_context));
    register_opcode(opcode_6502_clc::create(a_context));
    register_opcode(opcode_6502_cld::create(a_context));
    register_opcode(opcode_6502_cli::create(a_context));
    register_opcode(opcode_6502_clv::create(a_context));
    register_opcode(opcode_6502_cmp::create(a_context, model));
    register_opcode(opcode_6502_cpx::create(a_context));
    register_opcode(opcode_6502_cpy::create(a_context));
    register_opcode(opcode_6502_dea::create(a_context, model));
    register_opcode(opcode_6502_dec::create(a_context, model));
    register_opcode(opcode_6502_dex::create(a_context));
    register_opcode(opcode_6502_dey::create(a_context));
    register_opcode(opcode_6502_eor::create(a_context, model));
    register_opcode(opcode_6502_ina::create(a_context, model));
    register_opcode(opcode_6502_inc::create(a_context, model));
    register_opcode(opcode_6502_inx::create(a_context));
    register_opcode(opcode_6502_iny::create(a_context));
    register_opcode(opcode_6502_jmp::create(a_context, model));
    register_opcode(opcode_6502_jsr::create(a_context));
    register_opcode(opcode_6502_lda::create(a_context, model));
    register_opcode(opcode_6502_ldx::create(a_context));
    register_opcode(opcode_6502_ldy::create(a_context));
    register_opcode(opcode_6502_lsr::create(a_context));
    register_opcode(opcode_6502_nop::create(a_context));
    register_opcode(opcode_6502_ora::create(a_context, model));
    register_opcode(opcode_6502_pha::create(a_context));
    register_opcode(opcode_6502_php::create(a_context));
    register_opcode(opcode_6502_phx::create(a_context, model));
    register_opcode(opcode_6502_phy::create(a_context, model));
    register_opcode(opcode_6502_pla::create(a_context));
    register_opcode(opcode_6502_plp::create(a_context));
    register_opcode(opcode_6502_plx::create(a_context, model));
    register_opcode(opcode_6502_ply::create(a_context, model));
    register_opcode(opcode_6502_rmb::create(a_context, model));
    register_opcode(opcode_6502_rol::create(a_context));
    register_opcode(opcode_6502_ror::create(a_context));
    register_opcode(opcode_6502_rti::create(a_context));
    register_opcode(opcode_6502_rts::create(a_context));
    register_opcode(opcode_6502_sbc::create(a_context, model));
    register_opcode(opcode_6502_sec::create(a_context));
    register_opcode(opcode_6502_sed::create(a_context));
    register_opcode(opcode_6502_sei::create(a_context));
    register_opcode(opcode_6502_smb::create(a_context, model));
    register_opcode(opcode_6502_sta::create(a_context, model));
    register_opcode(opcode_6502_stp::create(a_context, model));
    register_opcode(opcode_6502_stx::create(a_context));
    register_opcode(opcode_6502_sty::create(a_context));
    register_opcode(opcode_6502_stz::create(a_context, model));
    register_opcode(opcode_6502_tax::create(a_context));
    register_opcode(opcode_6502_tay::create(a_context));
    register_opcode(opcode_6502_trb::create(a_context, model));
    register_opcode(opcode_6502_tsb::create(a_context, model));
    register_opcode(opcode_6502_tsx::create(a_context));
    register_opcode(opcode_6502_txa::create(a_context));
    register_opcode(opcode_6502_txs::create(a_context));
    register_opcode(opcode_6502_tya::create(a_context));
    register_opcode(opcode_6502_wai::create(a_context, model));

    location builtin("builtin", 0);
    register_special(expression_special_a::create(builtin));
    register_special(expression_special_x::create(builtin));
    register_special(expression_special_y::create(builtin));
}


personality::pointer
personality_6502::create(assembler *a_context, chip_model_t a_model)
{
    return pointer(new personality_6502(a_context, a_model));
}


personality::pointer
personality_6502::create_6502(assembler *a_context)
{
    return create(a_context, opcode_6502::model_6502);
}


personality::pointer
personality_6502::create_65c02(assembler *a_context)
{
    return create(a_context, opcode_6502::model_65c02);
}


personality::pointer
personality_6502::create_65c02s(assembler *a_context)
{
    return create(a_context, opcode_6502::model_65c02s);
}


personality::pointer
personality_6502::create_65c02s_wdc(assembler *a_context)
{
    return create(a_context, opcode_6502::model_65c02s_wdc);
}


mtype_t
personality_6502::get_mtype(void)
    const
{
    return mtype_6502; // Mos Technologoes 6502
}


unsigned char
personality_6502::get_one_byte_nop(void)
    const
{
    return 0xEA;
}


// vim: set ts=8 sw=4 et :
