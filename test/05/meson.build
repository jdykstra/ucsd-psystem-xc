cases = [
  ['t0500a', [compile_exe, disassemble_exe]],
  ['t0501a', [compile_exe, disassemble_exe]],
  ['t0502a', [compile_exe]],
  ['t0503a', [compile_exe, disassemble_exe]],
  ['t0505a', [compile_exe, disassemble_exe]],
  ['t0506a', [compile_exe]],
  ['t0507a', [compile_exe]],
  ['t0508a', [compile_exe, disassemble_exe]],
  ['t0509a', [compile_exe, disassemble_exe]],
  ['t0511a', [compile_exe, disassemble_exe]],
  ['t0512a', [compile_exe]],
  ['t0513a', [compile_exe]],
  ['t0515a', [compile_exe, disassemble_exe]],
  ['t0516a', [compile_exe, disassemble_exe]],
  ['t0517a', [compile_exe, disassemble_exe]],
  ['t0518a', [compile_exe]],
  ['t0519a', [compile_exe, disassemble_exe]],
  ['t0520a', [compile_exe, disassemble_exe]],
  ['t0521a', [compile_exe, disassemble_exe]],
  ['t0522a', [compile_exe, disassemble_exe]],
  ['t0523a', [compile_exe, disassemble_exe]],
  ['t0525a', [compile_exe, disassemble_exe]],
  ['t0526a', [compile_exe, disassemble_exe]],
  ['t0527a', [compile_exe, disassemble_exe]],
  ['t0528a', [assemble_exe, disassemble_exe]],
  ['t0529a', [assemble_exe, disassemble_exe]],
  ['t0530a', [compile_exe, disassemble_exe]],
  ['t0531a', [compile_exe, disassemble_exe]],
  ['t0532a', [compile_exe, disassemble_exe]],
  ['t0533a', [compile_exe, disassemble_exe]],
  ['t0534a', [compile_exe, disassemble_exe]],
  ['t0535a', [compile_exe, disassemble_exe]],
  ['t0536a', [compile_exe, disassemble_exe]],
  ['t0537a', [compile_exe, disassemble_exe]],
  ['t0539a', [compile_exe, disassemble_exe]],
  ['t0540a', [compile_exe, disassemble_exe]],
  ['t0541a', [compile_exe, disassemble_exe]],
  ['t0542a', [compile_exe, disassemble_exe]],
  ['t0543a', [compile_exe, disassemble_exe]],
  ['t0544a', [compile_exe]],
  ['t0545a', [compile_exe]],
  ['t0546a', [compile_exe, disassemble_exe]],
  ['t0547a', [compile_exe, disassemble_exe]],
  ['t0548a', [compile_exe, disassemble_exe]],
  ['t0549a', [compile_exe, disassemble_exe]],
  ['t0551a', [compile_exe, disassemble_exe]],
  ['t0552a', [compile_exe, disassemble_exe]],
  ['t0553a', [compile_exe, disassemble_exe]],
  ['t0555a', [compile_exe, disassemble_exe]],
  ['t0556a', [compile_exe]],
  ['t0557a', [compile_exe, disassemble_exe]],
  ['t0558a', [compile_exe]],
  ['t0559a', [compile_exe, libmap_exe, librarian_exe]],
  ['t0560a', [compile_exe, disassemble_exe]],
  ['t0562a', [compile_exe, disassemble_exe]],
  ['t0563a', [compile_exe, disassemble_exe]],
  ['t0564a', [compile_exe, disassemble_exe]],
  ['t0565a', [compile_exe, disassemble_exe]],
  ['t0566a', [compile_exe, disassemble_exe]],
  ['t0567a', [compile_exe, disassemble_exe]],
  ['t0568a', [compile_exe, disassemble_exe]],
  ['t0569a', [compile_exe, disassemble_exe]],
  ['t0570a', [compile_exe, disassemble_exe]],
  ['t0571a', [compile_exe, disassemble_exe]],
  ['t0572a', [compile_exe, disassemble_exe]],
  ['t0573a', [compile_exe]],
  ['t0574a', [compile_exe, disassemble_exe]],
  ['t0575a', [compile_exe, disassemble_exe]],
  ['t0576a', [compile_exe, disassemble_exe]],
  ['t0577a', [compile_exe]],
  ['t0578a', [compile_exe]],
  ['t0579a', [compile_exe]],
  ['t0580a', [compile_exe, disassemble_exe]],
  ['t0581a', [downcase_exe]],
  ['t0582a', [downcase_exe]],
  ['t0583a', [setup_exe]],
  ['t0584a', [setup_exe]],
  ['t0585a', [compile_exe, disassemble_exe]],
  ['t0586a', [compile_exe, disassemble_exe]],
  ['t0587a', [compile_exe, disassemble_exe]],
  ['t0589a', [compile_exe, disassemble_exe]],
  ['t0591a', [compile_exe, disassemble_exe]],
  ['t0592a', [compile_exe, disassemble_exe]],
  ['t0594a', [compile_exe, disassemble_exe]],
  ['t0595a', [compile_exe, disassemble_exe]],
  ['t0596a', [compile_exe]],
  ['t0597a', [compile_exe, disassemble_exe]],
  ['t0598a', [compile_exe]],
  ['t0599a', [compile_exe, disassemble_exe]],
]

foreach case : cases
  name = case[0]
  depends = case[1]
  test(name, sh, args : files(name + '.sh'), env : test_env, depends : depends)
endforeach
