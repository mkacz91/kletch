#include "prefix.h"

#include <lib/reference_fresnel.h>
using namespace kletch;

// Tests whether `ReferenceFresnel::eval_standard` gives accurate results
TEST(ReferenceFresnelTest, eval_standard)
{
    // Generated by misc/reference_fresnel_test_gen.py using scipy.special.fresnel
    real values[][3] =
    {
        {rl(0.0), rl(0.0), rl(0.0)},
        {rl(0.030303030303030304), rl(0.0303030239982492), rl(1.4569905606613524e-05)},
        {rl(0.06060606060606061), rl(0.0606058588533568), rl(0.00011655898502038013)},
        {rl(0.09090909090909091), rl(0.09090755885908858), rl(0.00039338277441318517)},
        {rl(0.12121212121212122), rl(0.12120566527484646), rl(0.0009324386221608842)},
        {rl(0.15151515151515152), rl(0.15149545025839467), rl(0.0018210693164923042)},
        {rl(0.18181818181818182), rl(0.18176916195536136), rl(0.0031464939927254095)},
        {rl(0.21212121212121213), rl(0.21201527215875277), rl(0.004995695453328303)},
        {rl(0.24242424242424243), rl(0.24221772883098466), rl(0.007455253102658109)},
        {rl(0.2727272727272727), rl(0.27235521687678216), rl(0.010611110826797538)},
        {rl(0.30303030303030304), rl(0.302400431863562), rl(0.014548269365796871)},
        {rl(0.33333333333333337), rl(0.3323193728996362), rl(0.01935039306462223)},
        {rl(0.36363636363636365), rl(0.3620706625889099), rl(0.025099321389145154)},
        {rl(0.3939393939393939), rl(0.39160490386775837), rl(0.03187447629825863)},
        {rl(0.42424242424242425), rl(0.4208640855730472), rl(0.03975215752048854)},
        {rl(0.4545454545454546), rl(0.44978105075948827), rl(0.048804719044403484)},
        {rl(0.48484848484848486), rl(0.47827904403987975), rl(0.059099621749993336)},
        {rl(0.5151515151515151), rl(0.5062713565124178), rl(0.07069835913695414)},
        {rl(0.5454545454545454), rl(0.5336610891017022), rl(0.0836552555982487)},
        {rl(0.5757575757575758), rl(0.5603410572968551), rl(0.09801613969263573)},
        {rl(0.6060606060606061), rl(0.5861938622286077), rl(0.1138168984309271)},
        {rl(0.6363636363636364), rl(0.6110921546784963), rl(0.1310819227406712)},
        {rl(0.6666666666666667), rl(0.6348991198321838), rl(0.14982245903226418)},
        {rl(0.696969696969697), rl(0.6574692112338295), rl(0.1700348871576242)},
        {rl(0.7272727272727273), rl(0.6786491623126122), rl(0.1916989510090369)},
        {rl(0.7575757575757576), rl(0.6982793028660809), rl(0.2147759745008708)},
        {rl(0.7878787878787878), rl(0.7161952058181112), rl(0.23920710262705908)},
        {rl(0.8181818181818182), rl(0.7322296862368308), rl(0.26491161456961565)},
        {rl(0.8484848484848485), rl(0.7462151698158448), rl(0.29178536328017834)},
        {rl(0.8787878787878788), rl(0.7579864416154021), rl(0.31969940335004654)},
        {rl(0.9090909090909092), rl(0.7673837776726071), rl(0.34849887605288765)},
        {rl(0.9393939393939394), rl(0.7742564519960041), rl(0.3780022268602481)},
        {rl(0.9696969696969697), rl(0.7784665993789094), rl(0.4080008361079175)},
        {rl(1.0), rl(0.779893400376823), rl(0.4382591473903547)},
        {rl(1.0303030303030303), rl(0.778437538753174), rl(0.4685153801896174)},
        {rl(1.0606060606060606), rl(0.7740258638556423), rl(0.4984829126718228)},
        {rl(1.0909090909090908), rl(0.7666161710067029), rl(0.5278524169473533)},
        {rl(1.1212121212121213), rl(0.7562019924724478), rl(0.5562948218205844)},
        {rl(1.1515151515151516), rl(0.7428172704558962), rl(0.5834651665956498)},
        {rl(1.1818181818181819), rl(0.7265407625479549), rl(0.6090073933467769)},
        {rl(1.2121212121212122), rl(0.7075000100326427), rl(0.6325601037769721)},
        {rl(1.2424242424242424), rl(0.6858746814251109), rl(0.6537632800735067)},
        {rl(1.2727272727272727), rl(0.6618990888270323), rl(0.6722659368904784)},
        {rl(1.303030303030303), rl(0.6358636644657468), rl(0.6877346338384895)},
        {rl(1.3333333333333335), rl(0.6081151806086329), rl(0.6998627350061447)},
        {rl(1.3636363636363638), rl(0.5790554994509419), rl(0.7083802547736796)},
        {rl(1.393939393939394), rl(0.5491386521118367), rl(0.7130640785794479)},
        {rl(1.4242424242424243), rl(0.5188660690190522), rl(0.7137482948579481)},
        {rl(1.4545454545454546), rl(0.48877981903226725), rl(0.7103343220232784)},
        {rl(1.4848484848484849), rl(0.45945376268699045), rl(0.7028004645212349)},
        {rl(1.5151515151515151), rl(0.43148258656920563), rl(0.6912104874570156)},
        {rl(1.5454545454545454), rl(0.4054687611506969), rl(0.6757207633636895)},
        {rl(1.5757575757575757), rl(0.3820075528390288), rl(0.6565855208701172)},
        {rl(1.6060606060606062), rl(0.36167032111827857), rl(0.6341597171179336)},
        {rl(1.6363636363636365), rl(0.3449864411264254), rl(0.6088990675653104)},
        {rl(1.6666666666666667), rl(0.3324243074386479), rl(0.5813568019288362)},
        {rl(1.696969696969697), rl(0.32437199170403824), rl(0.5521767766570221)},
        {rl(1.7272727272727273), rl(0.32111823950609675), rl(0.5220826649839462)},
        {rl(1.7575757575757576), rl(0.3228345937247709), rl(0.491863066720178)},
        {rl(1.7878787878787878), rl(0.3295595152057822), rl(0.4623525315559379)},
        {rl(1.8181818181818183), rl(0.3411854284662814), rl(0.4344086701084537)},
        {rl(1.8484848484848486), rl(0.35744964194705553), rl(0.4088857325321784)},
        {rl(1.878787878787879), rl(0.37793007057066147), rl(0.38660525921911415)},
        {rl(1.9090909090909092), rl(0.40204661540716097), rl(0.36832464347143445)},
        {rl(1.9393939393939394), rl(0.4290689247881653), rl(0.3547046810425847)},
        {rl(1.9696969696969697), rl(0.45813106906400825), rl(0.34627740273582397)},
        {rl(2.0), rl(0.48825340607534073), rl(0.34341567836369824)},
        {rl(2.0303030303030303), rl(0.5183715986698235), rl(0.34630622631741376)},
        {rl(2.0606060606060606), rl(0.5473723759441146), rl(0.3549277450357008)},
        {rl(2.090909090909091), rl(0.5741352179358283), rl(0.3690358833569377)},
        {rl(2.121212121212121), rl(0.5975787060598091), rl(0.3881566702137743)},
        {rl(2.1515151515151514), rl(0.6167098407538412), rl(0.4115898175561763)},
        {rl(2.1818181818181817), rl(0.6306742105235711), rl(0.43842298558417786)},
        {rl(2.2121212121212124), rl(0.6388045338543037), rl(0.467557654444143)},
        {rl(2.2424242424242427), rl(0.6406648209914043), rl(0.497746687449452)},
        {rl(2.272727272727273), rl(0.6360872509095522), rl(0.5276430127570351)},
        {rl(2.303030303030303), rl(0.6251988628534643), rl(0.5558581184818697)},
        {rl(2.3333333333333335), rl(0.6084353501885695), rl(0.5810282860974437)},
        {rl(2.3636363636363638), rl(0.5865396379299762), rl(0.6018857242713102)},
        {rl(2.393939393939394), rl(0.5605435343466216), rl(0.6173310642743312)},
        {rl(2.4242424242424243), rl(0.5317315675736631), rl(0.6265030992818034)},
        {rl(2.4545454545454546), rl(0.5015871295793445), rl(0.6288412565401221)},
        {rl(2.484848484848485), rl(0.4717222128163766), rl(0.6241361450680267)},
        {rl(2.515151515151515), rl(0.44379328068020385), rl(0.6125636767411378)},
        {rl(2.5454545454545454), rl(0.41940708401166626), rl(0.5946987565787885)},
        {rl(2.5757575757575757), rl(0.40002142865696244), rl(0.5715054007456541)},
        {rl(2.606060606060606), rl(0.3868469082593007), rl(0.544301364831845)},
        {rl(2.6363636363636362), rl(0.3807563317603389), rl(0.5146969167411294)},
        {rl(2.666666666666667), rl(0.3822088899060485), rl(0.4845092007470066)},
        {rl(2.6969696969696972), rl(0.3911959265282806), rl(0.4556556101452399)},
        {rl(2.7272727272727275), rl(0.4072144405569761), rl(0.43003158109446965)},
        {rl(2.757575757575758), rl(0.4292731117664018), rl(0.4093800784175765)},
        {rl(2.787878787878788), rl(0.45593373163930684), rl(0.3951615865212283)},
        {rl(2.8181818181818183), rl(0.48538849913947407), rl(0.3884344619645406)},
        {rl(2.8484848484848486), rl(0.515570836830659), rl(0.3897558773611276)},
        {rl(2.878787878787879), rl(0.5442943807930678), rl(0.39911314876043147)},
        {rl(2.909090909090909), rl(0.5694118346224903), rl(0.41589390031540396)},
        {rl(2.9393939393939394), rl(0.5889827280252058), rl(0.4389012597997394)},
        {rl(2.9696969696969697), rl(0.6014370769028471), rl(0.4664171597202558)},
        {rl(3.0), rl(0.6057207892976857), rl(0.49631299896737496)},
        {rl(-1), rl(-1), rl(-1)}
    };

    int i = 0;
    while (values[i][0] != rl(-1))
    {
        vec2r expected = vec2r(values[i][1], values[i][2]);
        vec2r actual = ReferenceFresnel::eval_standard(values[i][0]);
#       ifdef KLETCH_WITH_DOUBLE
            real const th = rl(1e-7);
#       else
            real const th = rl(1e-6);
#       endif
        ASSERT_LT(dist(expected, actual), th);
        ++i;
    }
}

// Tests whether `ReferenceFresnel::eval_bgk1` gives correct results. The accuracy is not important
// here as it is determined by the accuracy of the `eval_standard`.
TEST(ReferenceFresnelTest, eval_bgk1)
{
    // Generated by misc/reference_fresnel_test_gen_eval_standard.py using scipy.special.fresnel
    real values[][5] =
    {
        {rl(0), rl(0.01), rl(0), rl(0.0), rl(0.0)},
        {rl(0), rl(0.01), rl(0.5), rl(0.4999999218750057), rl(0.00020833331008184643)},
        {rl(0), rl(0.01), rl(1), rl(0.9999975000028936), rl(0.0016666636904785579)},
        {rl(0), rl(0.01), rl(2), rl(1.9999200014814678), rl(0.013332952385800834)},
        {rl(0), rl(0.5), rl(0), rl(0.0), rl(0.0)},
        {rl(0), rl(0.5), rl(0.5), rl(0.4998047228180872), rl(0.010413760591870398)},
        {rl(0), rl(0.5), rl(1), rl(0.9937680584295896), rl(0.08296204853709496)},
        {rl(0), rl(0.5), rl(2), rl(1.809048475800544), rl(0.6205366034467622)},
        {rl(0), rl(1), rl(0), rl(0.0), rl(0.0)},
        {rl(0), rl(1), rl(0.5), rl(0.49921931493660254), rl(0.020810093401773637)},
        {rl(0), rl(1), rl(1), rl(0.9752876882003446), rl(0.16371404737570058)},
        {rl(0), rl(1), rl(2), rl(1.3351936962943367), rl(0.9976237113254213)},
        {rl(0), rl(2), rl(0), rl(0.0), rl(0.0)},
        {rl(0), rl(2), rl(0.5), rl(0.4968840292147948), rl(0.04148102426854748)},
        {rl(0), rl(2), rl(1), rl(0.904524237900272), rl(0.3102683017233811)},
        {rl(0), rl(2), rl(2), rl(0.46146146243321634), rl(0.8047764893437559)},
        {rl(0), rl(-1), rl(0), rl(0.0), rl(0.0)},
        {rl(0), rl(-1), rl(0.5), rl(0.49921931493660254), rl(-0.020810093401773637)},
        {rl(0), rl(-1), rl(1), rl(0.9752876882003446), rl(-0.16371404737570058)},
        {rl(0), rl(-1), rl(2), rl(1.3351936962943367), rl(-0.9976237113254213)},
        {rl(0.1), rl(0.01), rl(0), rl(0.0), rl(0.0)},
        {rl(0.1), rl(0.01), rl(0.5), rl(0.4997838043224845), rl(0.012705569879501038)},
        {rl(0.1), rl(0.01), rl(1), rl(0.998206814490834), rl(0.0516198058244453)},
        {rl(0.1), rl(0.01), rl(2), rl(1.9846233911818452), rl(0.21249429393257174)},
        {rl(0.1), rl(0.5), rl(0), rl(0.0), rl(0.0)},
        {rl(0.1), rl(0.5), rl(0.5), rl(0.4992062004410789), rl(0.022895214240346524)},
        {rl(0.1), rl(0.5), rl(1), rl(0.9859138409407171), rl(0.13215342755284665)},
        {rl(0.1), rl(0.5), rl(2), rl(1.706905504767504), rl(0.7810550505325612)},
        {rl(0.1), rl(1), rl(0), rl(0.0), rl(0.0)},
        {rl(0.1), rl(1), rl(0.5), rl(0.4982316878749644), rl(0.03325937286785452)},
        {rl(0.1), rl(1), rl(1), rl(0.9614817027312353), rl(0.2111292797458744)},
        {rl(0.1), rl(1), rl(2), rl(1.190057080865786), rl(1.077606029037091)},
        {rl(0.1), rl(2), rl(0), rl(0.0), rl(0.0)},
        {rl(0.1), rl(2), rl(0.5), rl(0.49512455478233874), rl(0.05381759389994154)},
        {rl(0.1), rl(2), rl(1), rl(0.8802370493077847), rl(0.3514000258964215)},
        {rl(0.1), rl(2), rl(2), rl(0.38471188280760676), rl(0.7629075548230836)},
        {rl(0.1), rl(-1), rl(0), rl(0.0), rl(0.0)},
        {rl(0.1), rl(-1), rl(0.5), rl(0.49979172111972847), rl(-0.008329613764768084)},
        {rl(0.1), rl(-1), rl(1), rl(0.9859381109761441), rl(-0.11532234319180212)},
        {rl(0.1), rl(-1), rl(2), rl(1.472127032561173), rl(-0.8960147396358445)},
        {rl(1), rl(0.01), rl(0), rl(0.0), rl(0.0)},
        {rl(1), rl(0.01), rl(0.5), rl(0.47934949224266943), rl(0.1226103458903127)},
        {rl(1), rl(0.01), rl(1), rl(0.840353102244822), rl(0.46089152724416127)},
        {rl(1), rl(0.01), rl(2), rl(0.8969580757218283), rl(1.416840416843225)},
        {rl(1), rl(0.5), rl(0), rl(0.0), rl(0.0)},
        {rl(1), rl(0.5), rl(0.5), rl(0.4754498898204091), rl(0.1319829300827966)},
        {rl(1), rl(0.5), rl(1), rl(0.7817957523292163), rl(0.5146758777735522)},
        {rl(1), rl(0.5), rl(2), rl(0.3534529048065939), rl(1.2800167680918932)},
        {rl(1), rl(1), rl(0), rl(0.0), rl(0.0)},
        {rl(1), rl(1), rl(0.5), rl(0.47112574952683506), rl(0.14137514990018457)},
        {rl(1), rl(1), rl(1), rl(0.7156448264092076), rl(0.559276447496785)},
        {rl(1), rl(1), rl(2), rl(0.04441490109667405), rl(0.9131265367999604)},
        {rl(1), rl(2), rl(0), rl(0.0), rl(0.0)},
        {rl(1), rl(2), rl(0.5), rl(0.4614666984236483), rl(0.15957953034364677)},
        {rl(1), rl(2), rl(1), rl(0.5720708097099667), rl(0.6143219793706307)},
        {rl(1), rl(2), rl(2), rl(0.20130240092081364), rl(0.3501180301773843)},
        {rl(1), rl(-1), rl(0), rl(0.0), rl(0.0)},
        {rl(1), rl(-1), rl(0.5), rl(0.4863011077463971), rl(0.10282931823169354)},
        {rl(1), rl(-1), rl(1), rl(0.9343841633311667), rl(0.32390523209608907)},
        {rl(1), rl(-1), rl(2), rl(1.8687683266623334), rl(0.647810464192178)},
        {rl(2), rl(0.01), rl(0), rl(0.0), rl(0.0)},
        {rl(2), rl(0.01), rl(0.5), rl(0.42059591276643726), rl(0.22999824828267193)},
        {rl(2), rl(0.01), rl(1), rl(0.45310552979367474), rl(0.7081672820610695)},
        {rl(2), rl(0.01), rl(2), rl(-0.37901987812317167), rl(0.8169473483476466)},
        {rl(2), rl(0.5), rl(0), rl(0.0), rl(0.0)},
        {rl(2), rl(0.5), rl(0.5), rl(0.41363139055952686), rl(0.23717673639319095)},
        {rl(2), rl(0.5), rl(1), rl(0.37843137208119715), rl(0.7069902768239942)},
        {rl(2), rl(0.5), rl(2), rl(-0.26833626996685955), rl(0.4061930310613636)},
        {rl(2), rl(1), rl(0), rl(0.0), rl(0.0)},
        {rl(2), rl(1), rl(0.5), rl(0.40628097803094193), rl(0.24420748506630385)},
        {rl(2), rl(1), rl(1), rl(0.3054827634970845), rl(0.6945788504850394)},
        {rl(2), rl(1), rl(2), rl(0.0021502967772724924), rl(0.22123333067355028)},
        {rl(2), rl(2), rl(0), rl(0.0), rl(0.0)},
        {rl(2), rl(2), rl(0.5), rl(0.39089787616460814), rl(0.2573379388867761)},
        {rl(2), rl(2), rl(1), rl(0.17672645240329696), rl(0.6400083840459466)},
        {rl(2), rl(2), rl(2), rl(0.2808909174111476), rl(0.4200105492946367)},
        {rl(2), rl(-1), rl(0), rl(0.0), rl(0.0)},
        {rl(2), rl(-1), rl(0.5), rl(0.4341510445009667), rl(0.21434538495393246)},
        {rl(2), rl(-1), rl(1), rl(0.608498164911925), rl(0.6742904756777368)},
        {rl(2), rl(-1), rl(2), rl(0.35150004075791813), rl(1.6292461438868047)},
        {rl(-0.1), rl(0.01), rl(0), rl(0.0), rl(-0.0)},
        {rl(-0.1), rl(0.01), rl(0.5), rl(0.4997994249805797), rl(-0.012289215712792179)},
        {rl(-0.1), rl(0.01), rl(1), rl(0.9984565362970642), rl(-0.04829647246951786)},
        {rl(-0.1), rl(0.01), rl(2), rl(1.9886055074287874), rl(-0.1861476162264686)},
        {rl(-0.1), rl(0.5), rl(0), rl(0.0), rl(-0.0)},
        {rl(-0.1), rl(0.5), rl(0.5), rl(0.4999869792522448), rl(-0.0020833100819314773)},
        {rl(-0.1), rl(0.5), rl(1), rl(0.9983350751508521), rl(0.03327385236108011)},
        {rl(-0.1), rl(0.5), rl(2), rl(1.8899826556800772), rl(0.44548344421047387)},
        {rl(-0.1), rl(1), rl(0), rl(0.0), rl(-0.0)},
        {rl(-0.1), rl(1), rl(0.5), rl(0.49979172111972847), rl(0.008329613764768084)},
        {rl(-0.1), rl(1), rl(1), rl(0.9859381109761441), rl(0.11532234319180212)},
        {rl(-0.1), rl(1), rl(2), rl(1.472127032561173), rl(0.8960147396358445)},
        {rl(-0.1), rl(2), rl(0), rl(0.0), rl(-0.0)},
        {rl(-0.1), rl(2), rl(0.5), rl(0.49823245004978245), rl(0.029082324783842286)},
        {rl(-0.1), rl(2), rl(1), rl(0.9261566425413236), rl(0.26731653637301617)},
        {rl(-0.1), rl(2), rl(2), rl(0.5497667077236328), rl(0.8378089797596984)},
        {rl(-0.1), rl(-1), rl(0), rl(0.0), rl(-0.0)},
        {rl(-0.1), rl(-1), rl(0.5), rl(0.4982316878749644), rl(-0.03325937286785452)},
        {rl(-0.1), rl(-1), rl(1), rl(0.9614817027312353), rl(-0.2111292797458744)},
        {rl(-0.1), rl(-1), rl(2), rl(1.190057080865786), rl(-1.077606029037091)},
        {rl(-1), rl(0.01), rl(0), rl(0.0), rl(-0.0)},
        {rl(-1), rl(0.01), rl(0.5), rl(0.479501442442119), rl(-0.1222244672390409)},
        {rl(-1), rl(0.01), rl(1), rl(0.8425855404574243), rl(-0.45850019476588566)},
        {rl(-1), rl(0.01), rl(2), rl(0.9216521780138379), rl(-1.4153002074270757)},
        {rl(-1), rl(0.5), rl(0), rl(0.0), rl(-0.0)},
        {rl(-1), rl(0.5), rl(0.5), rl(0.4830449418570034), rl(-0.11269425206599655)},
        {rl(-1), rl(0.5), rl(1), rl(0.8928512795621704), rl(-0.39558204410599807)},
        {rl(-1), rl(0.5), rl(2), rl(1.499596609713972), rl(-1.186984444779239)},
        {rl(-1), rl(1), rl(0), rl(0.0), rl(-0.0)},
        {rl(-1), rl(1), rl(0.5), rl(0.4863011077463971), rl(-0.10282931823169354)},
        {rl(-1), rl(1), rl(1), rl(0.9343841633311667), rl(-0.32390523209608907)},
        {rl(-1), rl(1), rl(2), rl(1.8687683266623334), rl(-0.647810464192178)},
        {rl(-1), rl(2), rl(0), rl(0.0), rl(-0.0)},
        {rl(-1), rl(2), rl(0.5), rl(0.49169967769382117), rl(-0.08273959643900389)},
        {rl(-1), rl(2), rl(1), rl(0.9833993553876421), rl(-0.16547919287800775)},
        {rl(-1), rl(2), rl(2), rl(1.5554701650976088), rl(0.44884278649262294)},
        {rl(-1), rl(-1), rl(0), rl(0.0), rl(-0.0)},
        {rl(-1), rl(-1), rl(0.5), rl(0.47112574952683506), rl(-0.14137514990018457)},
        {rl(-1), rl(-1), rl(1), rl(0.7156448264092076), rl(-0.559276447496785)},
        {rl(-1), rl(-1), rl(2), rl(0.04441490109667405), rl(-0.9131265367999604)},
        {rl(-1000), rl(0), rl(0), rl(0), rl(0)}
    };

    int i = 0;
    while (values[i][0] != rl(-1000))
    {
        real k0 = values[i][0], k1 = values[i][1], s = values[i][2];
        vec2r expected = vec2r(values[i][3], values[i][4]);
        vec2r actual = ReferenceFresnel::eval_bgk1(k0, k1, s);
        ASSERT_LT(dist(expected, actual), rl(1e-3));
        ++i;
    }
}
