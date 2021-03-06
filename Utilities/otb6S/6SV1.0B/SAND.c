/* SAND.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
/* OTB patches: replace "f2c.h" by "otb_6S.h" */
/*#include "f2c.h"*/
#include "otb_6S.h"

/*<       subroutine sand(r) >*/
/* Subroutine */ int sand_(doublereal *r__)
{
    /* Initialized data */

    static doublereal sr[1501] = { 0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
	    0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
	    0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
	    0.,.0455,.091,.091,.091,.091,.091,.091,.091,.091,.091,.091,.091,
	    .093,.095,.095,.095,.095,.095,.095,.095,.095,.095,.095,.095,.095,
	    .095,.096,.097,.097,.097,.0985,.1,.1015,.103,.103,.103,.105,.107,
	    .107,.107,.107,.107,.107,.107,.1085,.11,.112,.114,.1155,.117,.119,
	    .121,.121,.121,.123,.125,.125,.125,.125,.125,.1265,.128,.127,.126,
	    .1285,.131,.1325,.134,.134,.134,.134,.134,.134,.134,.134,.134,
	    .134,.134,.1375,.141,.143,.145,.147,.149,.149,.149,.1515,.154,
	    .1555,.157,.1585,.16,.1615,.163,.163,.163,.1645,.166,.1675,.169,
	    .171,.173,.175,.177,.179,.181,.183,.185,.187,.189,.192,.195,.197,
	    .199,.2015,.204,.204,.204,.206,.208,.211,.214,.217,.22,.222,.224,
	    .226,.228,.2305,.233,.234,.235,.237,.239,.2405,.242,.244,.246,
	    .246,.246,.247,.248,.2495,.251,.2525,.254,.255,.256,.258,.26,
	    .2615,.263,.263,.263,.263,.263,.2645,.266,.268,.27,.2715,.273,
	    .2745,.276,.2775,.279,.279,.279,.2805,.282,.283,.284,.284,.284,
	    .284,.284,.285,.286,.286,.286,.286,.286,.288,.29,.291,.292,.292,
	    .292,.292,.292,.292,.292,.292,.292,.292,.292,.292,.292,.2935,.295,
	    .295,.295,.2965,.298,.298,.298,.2995,.301,.301,.301,.3035,.306,
	    .306,.306,.308,.31,.31,.31,.312,.314,.314,.314,.314,.314,.315,
	    .316,.316,.316,.316,.316,.316,.316,.316,.316,.3175,.319,.32,.321,
	    .321,.321,.321,.321,.321,.321,.3225,.324,.324,.324,.324,.324,.324,
	    .324,.323,.322,.322,.322,.322,.322,.321,.32,.32,.32,.3185,.317,
	    .317,.317,.317,.317,.3185,.32,.32,.32,.32,.32,.3215,.323,.323,
	    .323,.325,.327,.327,.327,.327,.327,.328,.329,.329,.329,.3305,.332,
	    .332,.332,.332,.332,.332,.332,.3325,.333,.334,.335,.3365,.338,
	    .3395,.341,.343,.345,.3465,.348,.3505,.353,.353,.353,.353,.353,
	    .353,.353,.354,.355,.355,.355,.355,.355,.355,.355,.354,.353,.353,
	    .353,.353,.353,.353,.353,.353,.353,.353,.353,.3545,.356,.356,.356,
	    .356,.356,.356,.356,.358,.36,.36,.36,.36,.36,.3615,.363,.363,.363,
	    .363,.363,.365,.367,.367,.367,.367,.367,.3685,.37,.37,.37,.37,.37,
	    .37,.37,.37,.37,.37,.37,.3685,.367,.367,.367,.3655,.364,.363,.362,
	    .362,.362,.362,.362,.3605,.359,.359,.359,.359,.359,.359,.359,.359,
	    .359,.361,.363,.363,.363,.363,.363,.363,.363,.363,.363,.364,.365,
	    .367,.369,.369,.369,.369,.369,.369,.369,.369,.369,.3705,.372,.372,
	    .372,.372,.372,.372,.372,.3735,.375,.375,.375,.375,.375,.375,.375,
	    .375,.375,.375,.375,.375,.375,.375,.375,.375,.375,.375,.375,.375,
	    .375,.375,.375,.375,.375,.375,.375,.375,.375,.376,.377,.377,.377,
	    .377,.377,.377,.377,.377,.377,.377,.377,.377,.377,.377,.377,.377,
	    .377,.377,.377,.377,.377,.377,.377,.378,.379,.379,.379,.379,.379,
	    .379,.379,.3805,.382,.382,.382,.382,.382,.382,.382,.3835,.385,
	    .385,.385,.385,.385,.385,.385,.385,.385,.385,.385,.3865,.388,.388,
	    .388,.388,.388,.388,.388,.388,.388,.388,.388,.388,.388,.3895,.391,
	    .391,.391,.391,.391,.391,.391,.391,.391,.391,.391,.391,.391,.391,
	    .391,.391,.391,.391,.391,.391,.391,.3925,.394,.394,.394,.394,.394,
	    .394,.394,.394,.394,.394,.394,.394,.394,.395,.396,.396,.396,.396,
	    .396,.396,.396,.396,.396,.396,.396,.3945,.393,.393,.393,.393,.393,
	    .393,.393,.391,.389,.3875,.386,.386,.386,.384,.382,.382,.382,.38,
	    .378,.378,.378,.376,.374,.3715,.369,.369,.369,.369,.369,.37,.371,
	    .371,.371,.371,.371,.371,.371,.371,.371,.371,.371,.371,.371,.371,
	    .371,.371,.371,.371,.371,.3725,.374,.374,.374,.3755,.377,.377,
	    .377,.377,.377,.377,.377,.378,.379,.379,.379,.379,.379,.379,.379,
	    .381,.383,.384,.385,.3865,.388,.388,.388,.388,.388,.39,.392,.392,
	    .392,.392,.392,.3935,.395,.395,.395,.395,.395,.395,.395,.395,.395,
	    .394,.393,.393,.393,.393,.393,.3905,.388,.388,.388,.388,.388,.388,
	    .388,.388,.388,.3865,.385,.385,.385,.385,.385,.383,.381,.381,.381,
	    .381,.381,.381,.381,.381,.381,.381,.381,.381,.381,.381,.381,.3775,
	    .374,.374,.374,.374,.374,.374,.374,.374,.374,.374,.374,.374,.374,
	    .374,.374,.373,.372,.3705,.369,.369,.369,.369,.369,.369,.369,.369,
	    .369,.369,.369,.369,.369,.369,.369,.369,.369,.369,.369,.369,.369,
	    .369,.369,.369,.369,.369,.369,.369,.369,.369,.369,.369,.369,.369,
	    .369,.369,.369,.369,.369,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
	    0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
	    0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
	    0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
	    0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
	    0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
	    0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
	    0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
	    0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
	    0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
	    0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
	    0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
	    0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
	    0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
	    0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
	    0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
	    0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
	    0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
	    0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
	    0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
	    0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
	    0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
	    0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
	    0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
	    0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
	    0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
	    0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
	    0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
	    0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
	    0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
	    0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
	    0.,0.,0.,0.,0.,0.,0. };

    integer i__;

/*<       real r,sr >*/
/*<       integer l,i >*/
/*<       dimension sr(1501),r(1501) >*/

/*     sand average reflectance */
/*     warning : values of dry sand ground reflectance are given */
/*     between 0.4 and 2.2 microns. outside this interval the */
/*     values are set to 0. */

/*<    >*/
    /* Parameter adjustments */
    --r__;

    /* Function Body */
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<       do 1 i=1,1501 >*/
    for (i__ = 1; i__ <= 1501; ++i__) {
/*<       r(i)=sr(i) >*/
	r__[i__] = sr[i__ - 1];
/*<     1 continue >*/
/* L1: */
    }
/*<       return >*/
    return 0;
/*<       end >*/
} /* sand_ */

#ifdef __cplusplus
	}
#endif
