LIBCSITTYFNSINDEPENDENCE:
    Port fns type printf fprintf sprintf atoi atoc etc. to your own faster and less portable(but faster) implementations. Not all, fns like free allocs and some other good fns may stay.
    LIBCSITTYFNSINDEPENDENCE is here tag comment that will be above functions that have thoes fns.
    Bellow that comment there will be list of fns to port AND PORT ONLY THOES THAT ARE LISTED
    If ported fns(I mean libs own memcpy) are considered by you to be good enought and usefull enought to be public than make them public in eutils.c, otherwise make them in private/common_utils.c
