#ifndef __NTP_H__
#define __NTP_H__

typedef struct {
	union {
		unsigned int Xl_ui;
		int Xl_i;
	} Ul_i;
	union {
		unsigned int Xl_uf;
		int Xl_f;
	} Ul_f;
} l_fp;

#define l_ui	Ul_i.Xl_ui		/* unsigned integral part */
#define	l_i	    Ul_i.Xl_i		/* signed integral part */
#define	l_uf	Ul_f.Xl_uf		/* unsigned fractional part */
#define	l_f	    Ul_f.Xl_f		/* signed fractional part */

struct ntp_pkt {
	unsigned char li_vn_mode;	  /* contains leap indicator, version and mode */
	unsigned char stratum;		  /* peer's stratum */
	unsigned char ppoll;		  /* the peer polling interval */
	unsigned char precision;	  /* peer clock precision */
	unsigned int  rootdelay;      /* distance to primary clock */
	unsigned int  rootdispersion; /* clock dispersion */
	unsigned int  refid;		  /* reference clock ID */
	l_fp reftime;		          /* time peer clock was last updated */
	l_fp org;		              /* originate time stamp */
	l_fp rec;		              /* receive time stamp */
	l_fp xmt;		              /* transmit time stamp */
};  

#define  NTP_VERSION  3

/*
 * Values for peer.leap, sys_leap
 */
#define	LEAP_NOWARNING	0x0	/* normal, no leap second warning */
#define	LEAP_ADDSECOND	0x1	/* last minute of day has 61 seconds */
#define	LEAP_DELSECOND	0x2	/* last minute of day has 59 seconds */
#define	LEAP_NOTINSYNC	0x3	/* overload, clock is free running */

/*
 * Values for peer.mode
 */
#define	MODE_UNSPEC	    0	/* unspecified (probably old NTP version) */
#define	MODE_ACTIVE	    1	/* symmetric active */
#define	MODE_PASSIVE	2	/* symmetric passive */
#define	MODE_CLIENT	    3	/* client mode */
#define	MODE_SERVER	    4	/* server mode */
#define	MODE_BROADCAST	5	/* broadcast mode */
#define	MODE_CONTROL	6	/* control mode packet */
#define	MODE_PRIVATE	7	/* implementation defined function */
#define	MODE_BCLIENT	8	/* a pseudo mode, used internally */
#define MODE_MCLIENT	9	/* multicast mode, used internally */

/*
 * Stuff for extracting things from li_vn_mode
 */
#define	PKT_MODE(li_vn_mode)	((unsigned char)((li_vn_mode) & 0x7))
#define	PKT_VERSION(li_vn_mode)	((unsigned char)(((li_vn_mode) >> 3) & 0x7))
#define	PKT_LEAP(li_vn_mode)	((unsigned char)(((li_vn_mode) >> 6) & 0x3))

#define	PKT_LI_VN_MODE(li, vn, md) \
	((unsigned char)((((li) << 6) & 0xc0) | (((vn) << 3) & 0x38) | ((md) & 0x7)))

/*
 * Since ntpdate isn't aware of some of the things that normally get
 * put in an NTP packet, we fix some values.
 */
#define	FP_SECOND	(0x10000)

#define	NTPDATE_PRECISION	(-6)		/* use this precision */
#define	NTPDATE_DISTANCE	FP_SECOND	/* distance is 1 sec */
#define	NTPDATE_DISP		FP_SECOND	/* so is the dispersion */
#define	NTPDATE_REFID		(0)		    /* reference ID to use */


/*
 * Dealing with stratum.  0 gets mapped to 16 incoming, and back to 0
 * on output.
 */
#define	NTP_MAXSTRATUM	    ((unsigned char)15) /* max stratum, infinity a la Bellman-Ford */
#define	STRATUM_PKT_UNSPEC  ((unsigned char)0) /* unspecified in packet */
#define	STRATUM_UNSPEC	    ((unsigned char)(NTP_MAXSTRATUM + (unsigned char)1)) /* unspecified */

#define	PKT_TO_STRATUM(s)	((unsigned char)(((s) == (STRATUM_PKT_UNSPEC)) ?\
				(STRATUM_UNSPEC) : (s)))
#define	STRATUM_TO_PKT(s)	((unsigned char)(((s) == (STRATUM_UNSPEC)) ?\
				(STRATUM_PKT_UNSPEC) : (s)))

/*
 *NTP protocol pool parameters.
 */
#define NTP_UNREACH	   16	/* poll interval backoff count */
#define NTP_MINDPOLL    6	/* log2 default min poll interval (64 s) */
#define NTP_MAXDPOLL   10	/* log2 default max poll interval (~17 m) */
#define	NTP_MINPOLL	    4	/* log2 min poll interval (16 s) */
#define	NTP_SHIFT	    8	/* 8 suitable for crystal time base */


#define	L_CLR(v)	((v)->l_ui = (v)->l_uf = 0)

void ntp_server_test();

#endif
