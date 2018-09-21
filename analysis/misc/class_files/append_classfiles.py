# edit the base class file name
cf_basename = "v8.02/SS" # .cc, .h
# copy the pattern for the following branch (make sure the type is correct)
branch_to_copy = "met3p0"
branch_to_copy_type = "float"
# and use this pattern to squeeze in the following branches (and associated types)
branches_to_add = [
        ("bdt_nbtags", "float"),
        ("bdt_njets", "float"),
        ("bdt_met", "float"),
        ("bdt_ptl2", "float"),
        ("bdt_nlb40", "float"),
        ("bdt_ntb40", "float"),
        ("bdt_nleps", "float"),
        ("bdt_htb", "float"),
        ("bdt_ml1j1", "float"),
        ("bdt_dphil1l2", "float"),
        ("bdt_maxmjoverpt", "float"),
        ("bdt_detal1l2", "float"),
        ("bdt_q1", "float"),
        ("bdt_ptj1", "float"),
        ("bdt_ptj6", "float"),
        ("bdt_ptj7", "float"),
        ("bdt_ptj8", "float"),
        ("bdt_ptl1", "float"),
        ("bdt_ptl3", "float"),
        ]

with open("{}_new.h".format(cf_basename), "w") as fhout:
    with open("{}.h".format(cf_basename), "r") as fhin:
        for line in fhin:
            if "{}_isLoaded;".format(branch_to_copy) in line:
                fhout.write(line)
                for bname,btype in branches_to_add:
                    fhout.write("  {} {}_;\n".format(btype,bname))
                    fhout.write("  TBranch *{}_branch;\n".format(bname))
                    fhout.write("  bool {}_isLoaded;\n".format(bname))
            elif "const {} &{}();".format(branch_to_copy_type,branch_to_copy) in line:
                fhout.write(line)
                for bname,btype in branches_to_add:
                    fhout.write("  const {} &{}();\n".format(btype,bname))
            else:
                fhout.write(line)


with open("{}_new.cc".format(cf_basename), "w") as fhout:
    with open("{}.cc".format(cf_basename), "r") as fhin:
        for line in fhin:
            if "{}_branch = 0;".format(branch_to_copy) == line.strip():
                for bname,btype in branches_to_add:
                    fhout.write("""
  {0}_branch = 0;
  if (tree->GetBranch(\"{0}\") != 0) {{
    {0}_branch = tree->GetBranch(\"{0}\");
    if ({0}_branch) {{ {0}_branch->SetAddress(&{0}_); }}
  }}
""".format(bname))
                fhout.write(line)

            elif "{}_isLoaded = false;".format(branch_to_copy) == line.strip():
                fhout.write(line)
                for bname,btype in branches_to_add:
                    fhout.write("  {}_isLoaded = false;\n".format(bname))

            elif "const {0} &{1}() {{ return samesign.{1}(); }}".format(branch_to_copy_type,branch_to_copy) == line.strip():
                fhout.write(line)
                for bname,btype in branches_to_add:
                    fhout.write("  const {0} &{1}() {{ return samesign.{1}(); }}\n".format(btype,bname))

            elif "if ({0}_branch != 0) {0}();".format(branch_to_copy) == line.strip():
                fhout.write(line)
                for bname,btype in branches_to_add:
                    fhout.write("  if ({0}_branch != 0) {0}();\n".format(bname))

            elif "const {} &SSAG::{}()".format(branch_to_copy_type,branch_to_copy) == line.strip():
                for bname,btype in branches_to_add:
                    fhout.write("""
const {0} &SSAG::{1}() {{
  if (not {1}_isLoaded) {{
    if ({1}_branch != 0) {{
      {1}_branch->GetEntry(index);
    }} else {{
      printf("branch {1}_branch does not exist!\\n");
      exit(1);
    }}
    {1}_isLoaded = true;
  }}
  return {1}_;
}}
""".format(btype,bname))
                fhout.write(line)

            else:
                fhout.write(line)

