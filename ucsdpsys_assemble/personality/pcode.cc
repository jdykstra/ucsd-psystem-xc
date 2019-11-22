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

#include <lib/pcode.h>

#include <ucsdpsys_assemble/opcode/pcode/abi.h>
#include <ucsdpsys_assemble/opcode/pcode/abr.h>
#include <ucsdpsys_assemble/opcode/pcode/adi.h>
#include <ucsdpsys_assemble/opcode/pcode/adj.h>
#include <ucsdpsys_assemble/opcode/pcode/adr.h>
#include <ucsdpsys_assemble/opcode/pcode/bpt.h>
#include <ucsdpsys_assemble/opcode/pcode/cbp.h>
#include <ucsdpsys_assemble/opcode/pcode/cgp.h>
#include <ucsdpsys_assemble/opcode/pcode/chk.h>
#include <ucsdpsys_assemble/opcode/pcode/cip.h>
#include <ucsdpsys_assemble/opcode/pcode/clp.h>
#include <ucsdpsys_assemble/opcode/pcode/csp.h>
#include <ucsdpsys_assemble/opcode/pcode/cxp.h>
#include <ucsdpsys_assemble/opcode/pcode/dif.h>
#include <ucsdpsys_assemble/opcode/pcode/dvi.h>
#include <ucsdpsys_assemble/opcode/pcode/dvr.h>
#include <ucsdpsys_assemble/opcode/pcode/equ.h>
#include <ucsdpsys_assemble/opcode/pcode/equi.h>
#include <ucsdpsys_assemble/opcode/pcode/flo.h>
#include <ucsdpsys_assemble/opcode/pcode/flt.h>
#include <ucsdpsys_assemble/opcode/pcode/geq.h>
#include <ucsdpsys_assemble/opcode/pcode/geqi.h>
#include <ucsdpsys_assemble/opcode/pcode/gtr.h>
#include <ucsdpsys_assemble/opcode/pcode/gtri.h>
#include <ucsdpsys_assemble/opcode/pcode/inc.h>
#include <ucsdpsys_assemble/opcode/pcode/ind.h>
#include <ucsdpsys_assemble/opcode/pcode/inn.h>
#include <ucsdpsys_assemble/opcode/pcode/int.h>
#include <ucsdpsys_assemble/opcode/pcode/ixa.h>
#include <ucsdpsys_assemble/opcode/pcode/ixp.h>
#include <ucsdpsys_assemble/opcode/pcode/ixs.h>
#include <ucsdpsys_assemble/opcode/pcode/lae.h>
#include <ucsdpsys_assemble/opcode/pcode/land.h>
#include <ucsdpsys_assemble/opcode/pcode/lao.h>
#include <ucsdpsys_assemble/opcode/pcode/lda.h>
#include <ucsdpsys_assemble/opcode/pcode/ldb.h>
#include <ucsdpsys_assemble/opcode/pcode/ldc.h>
#include <ucsdpsys_assemble/opcode/pcode/ldci.h>
#include <ucsdpsys_assemble/opcode/pcode/ldcn.h>
#include <ucsdpsys_assemble/opcode/pcode/lde.h>
#include <ucsdpsys_assemble/opcode/pcode/ldl.h>
#include <ucsdpsys_assemble/opcode/pcode/ldm.h>
#include <ucsdpsys_assemble/opcode/pcode/ldo.h>
#include <ucsdpsys_assemble/opcode/pcode/ldp.h>
#include <ucsdpsys_assemble/opcode/pcode/leq.h>
#include <ucsdpsys_assemble/opcode/pcode/leqi.h>
#include <ucsdpsys_assemble/opcode/pcode/les.h>
#include <ucsdpsys_assemble/opcode/pcode/lesi.h>
#include <ucsdpsys_assemble/opcode/pcode/lla.h>
#include <ucsdpsys_assemble/opcode/pcode/lnot.h>
#include <ucsdpsys_assemble/opcode/pcode/lod.h>
#include <ucsdpsys_assemble/opcode/pcode/lor.h>
#include <ucsdpsys_assemble/opcode/pcode/lpa.h>
#include <ucsdpsys_assemble/opcode/pcode/lsa.h>
#include <ucsdpsys_assemble/opcode/pcode/modi.h>
#include <ucsdpsys_assemble/opcode/pcode/mov.h>
#include <ucsdpsys_assemble/opcode/pcode/mpi.h>
#include <ucsdpsys_assemble/opcode/pcode/mpr.h>
#include <ucsdpsys_assemble/opcode/pcode/neq.h>
#include <ucsdpsys_assemble/opcode/pcode/neqi.h>
#include <ucsdpsys_assemble/opcode/pcode/ngi.h>
#include <ucsdpsys_assemble/opcode/pcode/ngr.h>
#include <ucsdpsys_assemble/opcode/pcode/nop.h>
#include <ucsdpsys_assemble/opcode/pcode/rbp.h>
#include <ucsdpsys_assemble/opcode/pcode/rbp.h>
#include <ucsdpsys_assemble/opcode/pcode/rnp.h>
#include <ucsdpsys_assemble/opcode/pcode/sas.h>
#include <ucsdpsys_assemble/opcode/pcode/sbi.h>
#include <ucsdpsys_assemble/opcode/pcode/sbr.h>
#include <ucsdpsys_assemble/opcode/pcode/sgs.h>
#include <ucsdpsys_assemble/opcode/pcode/sind.h>
#include <ucsdpsys_assemble/opcode/pcode/sldc.h>
#include <ucsdpsys_assemble/opcode/pcode/sldl.h>
#include <ucsdpsys_assemble/opcode/pcode/sldo.h>
#include <ucsdpsys_assemble/opcode/pcode/sqi.h>
#include <ucsdpsys_assemble/opcode/pcode/sqr.h>
#include <ucsdpsys_assemble/opcode/pcode/sro.h>
#include <ucsdpsys_assemble/opcode/pcode/srs.h>
#include <ucsdpsys_assemble/opcode/pcode/stb.h>
#include <ucsdpsys_assemble/opcode/pcode/ste.h>
#include <ucsdpsys_assemble/opcode/pcode/stl.h>
#include <ucsdpsys_assemble/opcode/pcode/stm.h>
#include <ucsdpsys_assemble/opcode/pcode/sto.h>
#include <ucsdpsys_assemble/opcode/pcode/stp.h>
#include <ucsdpsys_assemble/opcode/pcode/str.h>
#include <ucsdpsys_assemble/opcode/pcode/uni.h>
#include <ucsdpsys_assemble/opcode/pcode/xit.h>
#include <ucsdpsys_assemble/personality/pcode.h>


personality_pcode::~personality_pcode()
{
}


personality_pcode::personality_pcode(
    assembler *a_context,
    byte_sex_t a_endian
) :
    context(a_context),
    endian(a_endian)
{
    register_opcode(opcode_pcode_abi::create(a_context));
    register_opcode(opcode_pcode_abr::create(a_context));
    register_opcode(opcode_pcode_adi::create(a_context));
    register_opcode(opcode_pcode_adj::create(a_context));
    register_opcode(opcode_pcode_adr::create(a_context));
    register_opcode(opcode_pcode_bpt::create(a_context));
    register_opcode(opcode_pcode_cbp::create(a_context));
    register_opcode(opcode_pcode_cgp::create(a_context));
    register_opcode(opcode_pcode_chk::create(a_context));
    register_opcode(opcode_pcode_cip::create(a_context));
    register_opcode(opcode_pcode_clp::create(a_context));
    register_opcode(opcode_pcode_csp::create(a_context));
    register_opcode(opcode_pcode_cxp::create(a_context));
    register_opcode(opcode_pcode_dif::create(a_context));
    register_opcode(opcode_pcode_dvi::create(a_context));
    register_opcode(opcode_pcode_dvr::create(a_context));
    register_opcode(opcode_pcode_equ::create(a_context));
    register_opcode(opcode_pcode_equi::create(a_context));
    register_opcode(opcode_pcode_flo::create(a_context));
    register_opcode(opcode_pcode_flt::create(a_context));
    register_opcode(opcode_pcode_geq::create(a_context));
    register_opcode(opcode_pcode_geqi::create(a_context));
    register_opcode(opcode_pcode_gtr::create(a_context));
    register_opcode(opcode_pcode_gtri::create(a_context));
    register_opcode(opcode_pcode_inc::create(a_context));
    register_opcode(opcode_pcode_ind::create(a_context));
    register_opcode(opcode_pcode_inn::create(a_context));
    register_opcode(opcode_pcode_int::create(a_context));
    register_opcode(opcode_pcode_ixa::create(a_context));
    register_opcode(opcode_pcode_ixp::create(a_context));
    register_opcode(opcode_pcode_ixs::create(a_context));
    register_opcode(opcode_pcode_lae::create(a_context));
    register_opcode(opcode_pcode_land::create(a_context));
    register_opcode(opcode_pcode_lao::create(a_context));
    register_opcode(opcode_pcode_lda::create(a_context));
    register_opcode(opcode_pcode_ldb::create(a_context));
    register_opcode(opcode_pcode_ldc::create(a_context));
    register_opcode(opcode_pcode_ldci::create(a_context));
    register_opcode(opcode_pcode_ldcn::create(a_context));
    register_opcode(opcode_pcode_lde::create(a_context));
    register_opcode(opcode_pcode_ldl::create(a_context));
    register_opcode(opcode_pcode_ldm::create(a_context));
    register_opcode(opcode_pcode_ldo::create(a_context));
    register_opcode(opcode_pcode_ldp::create(a_context));
    register_opcode(opcode_pcode_leq::create(a_context));
    register_opcode(opcode_pcode_leqi::create(a_context));
    register_opcode(opcode_pcode_les::create(a_context));
    register_opcode(opcode_pcode_lesi::create(a_context));
    register_opcode(opcode_pcode_lla::create(a_context));
    register_opcode(opcode_pcode_lnot::create(a_context));
    register_opcode(opcode_pcode_lod::create(a_context));
    register_opcode(opcode_pcode_lor::create(a_context));
    register_opcode(opcode_pcode_lpa::create(a_context));
    register_opcode(opcode_pcode_lsa::create(a_context));
    register_opcode(opcode_pcode_modi::create(a_context));
    register_opcode(opcode_pcode_mov::create(a_context));
    register_opcode(opcode_pcode_mpi::create(a_context));
    register_opcode(opcode_pcode_mpr::create(a_context));
    register_opcode(opcode_pcode_neq::create(a_context));
    register_opcode(opcode_pcode_neqi::create(a_context));
    register_opcode(opcode_pcode_ngi::create(a_context));
    register_opcode(opcode_pcode_ngr::create(a_context));
    register_opcode(opcode_pcode_nop::create(a_context));
    register_opcode(opcode_pcode_rbp::create(a_context));
    register_opcode(opcode_pcode_rbp::create(a_context));
    register_opcode(opcode_pcode_rnp::create(a_context));
    register_opcode(opcode_pcode_sas::create(a_context));
    register_opcode(opcode_pcode_sbi::create(a_context));
    register_opcode(opcode_pcode_sbr::create(a_context));
    register_opcode(opcode_pcode_sgs::create(a_context));
    register_opcode(opcode_pcode_sind::create(a_context));
    register_opcode(opcode_pcode_sldc::create(a_context));
    register_opcode(opcode_pcode_sldl::create(a_context));
    register_opcode(opcode_pcode_sldo::create(a_context));
    register_opcode(opcode_pcode_sqi::create(a_context));
    register_opcode(opcode_pcode_sqr::create(a_context));
    register_opcode(opcode_pcode_sro::create(a_context));
    register_opcode(opcode_pcode_srs::create(a_context));
    register_opcode(opcode_pcode_stb::create(a_context));
    register_opcode(opcode_pcode_ste::create(a_context));
    register_opcode(opcode_pcode_stl::create(a_context));
    register_opcode(opcode_pcode_stm::create(a_context));
    register_opcode(opcode_pcode_sto::create(a_context));
    register_opcode(opcode_pcode_stp::create(a_context));
    register_opcode(opcode_pcode_str::create(a_context));
    register_opcode(opcode_pcode_uni::create(a_context));
    register_opcode(opcode_pcode_xit::create(a_context));
}


personality_pcode::pointer
personality_pcode::create(assembler *a_context, byte_sex_t a_endian)
{
    return pointer(new personality_pcode(a_context, a_endian));
}


personality::pointer
personality_pcode::create_le(assembler *a_context)
{
    return create(a_context, little_endian);
}


personality::pointer
personality_pcode::create_be(assembler *a_context)
{
    return create(a_context, big_endian);
}


byte_sex_t
personality_pcode::get_byte_sex(void)
    const
{
    return endian;
}


mtype_t
personality_pcode::get_mtype(void)
    const
{
    return mtype_pcode_from_byte_sex(endian);
}


int
personality_pcode::get_default_radix(void)
    const
{
    return 10;
}


unsigned char
personality_pcode::get_one_byte_nop(void)
    const
{
    return NOP;
}


// vim: set ts=8 sw=4 et :
