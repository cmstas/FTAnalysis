

def write_table(data, bgs, outname=None):
    from pytable import Table
    tab = Table()
    sumbgs = sum(bgs)
    databg = data/sumbgs
    procs = bgs+[sumbgs,data,databg]
    cnames = [bg.get_attr("label") for bg in bgs] + ["Total bkg","Data", "Data/bkg"]
    tab.set_column_names(["bin"]+cnames)
    sep = "+-"
    precision = 2
    if "tt_isr" in outname and "nisrjets" in outname:
        precision = 5
    binpairs = zip(data.edges[:-1],data.edges[1:])
    tab.set_theme_basic()
    for ibin,binrow in enumerate(binpairs):
        row = ["[{},{}]".format(*binrow)]
        for iproc,proc in enumerate(procs):
            cent = max(proc.counts[ibin],0.)
            err = proc.errors[ibin]
            row.append(("{0:5.%if} {1}{2:%i.%if}" % (precision,precision+3,precision)).format(cent,sep,err))
        tab.add_row(row)
    tab.add_line()

    row = ["total"]
    for iproc,proc in enumerate(procs):
        if iproc == len(procs)-1:
            totbg = E(sum(sumbgs.counts), np.sum(sumbgs.errors**2.)**0.5)
            totdata = E(sum(data.counts))
            ratio = totdata/ totbg
            cent, err = ratio[0], ratio[1]
        else:
            cent = sum(proc.counts)
            err = np.sum(proc.errors**2.)**0.5
        row.append(("{0:5.%if} {1}{2:%i.%if}" % (precision,precision+3,precision)).format(cent,sep,err))
    tab.add_row(row)

    if outname:
        with open(outname,"w") as fhout:
            towrite = "".join(tab.get_table_string(show_row_separators=False,show_alternating=False))
            fhout.write(towrite)
            parts = towrite.split("\n")
            header = parts[:3]
            binparts = parts[3:-4]
            total = parts[-4:-1]
            table_info = { "header":"<br>".join(header), "bins":binparts, "total":"<br>".join(total) }
            return table_info

def get_bin_info(bin_coords, table_info):
    info = { "xedges": [], "yedges": [], "table": table_info }
    for ipair,pair in enumerate(zip(bin_coords[::2],bin_coords[1::2])):
        xlow,xhigh = round(pair[0][0],5), round(pair[1][0],5)
        ylow,yhigh = round(pair[0][1],5), round(pair[1][1],5)
        info["xedges"].append([xlow,xhigh])
        info["yedges"].append([ylow,yhigh])
    return info

