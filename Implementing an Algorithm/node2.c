#define INFINITY 999
#define TOTAL 4
#define ME 2

static int cost[TOTAL][TOTAL];	/* distance table: cost[i][j] represent the cost to j through i */
static int routing[TOTAL];		/* rounting table: the best path to node i */

/* a rtpkt is the packet sent from one routing update process to
   another via the call tolayer3() */
struct rtpkt {
	int sourceid;       /* id of sending router sending this pkt */
	int destid;         /* id of router to which pkt being sent
                         (must be an immediate neighbor) */
	int mincost[4];    /* min cost to node 0 ... 3 */
};

/* broadcast: broadcast routing packet to neighbor */
static broadcast()
{
	int i, j;
	for (i = 0; i < TOTAL; i++)
		if (i != ME && cost[i][i] != INFINITY) {
			/* Construct routing packets */
			struct rtpkt packet;
			packet.sourceid = ME;
			packet.destid = i;
			for (j = 0; j < TOTAL; j++)
				packet.mincost[j] = cost[routing[j]][j];
			/* Send packet */
			tolayer2(packet);
		}
}

printdt2()
{
	int i, j;
	printf("The distance table of node %d:\n", ME);
	for (i = 0; i < TOTAL; i++) {
		for (j = 0; j < TOTAL; j++)
			printf("%4d", cost[i][j]);
		printf("\n");
	}
}

rtinit2()
{
	int i, j;
	/* Init distance table */
	for (i = 0; i < TOTAL; i++)
		for (j = 0; j < TOTAL; j++)
			cost[i][j] = INFINITY;
	/* Init routing table */
	for (i = 0; i < TOTAL; i++)
		routing[i] = i;
	/* Count costs to neighbor */
	cost[ME][ME] = 0;
	cost[0][0] = 3;
	cost[1][1] = 1;
	cost[3][3] = 2;
	/* Broadcast */
	broadcast();
}

rtupdate2(rcvdpkt)
struct rtpkt *rcvdpkt;
{
	int i, j, source = rcvdpkt->sourceid, changed = 0;
	for (i = 0; i < TOTAL; i++) {
		/* Compute new cost */
		int new_cost = cost[source][source] + rcvdpkt->mincost[i];
		/* Save old cost */
		int old_cost = cost[routing[i]][i];
		/* Update distance table */
		cost[source][i] = new_cost;
		/* Update routing table */
		if (new_cost < old_cost) {
			changed = 1;
			routing[i] = source;
		} else if (routing[i] == source && new_cost > old_cost) {
			/* Find the best path */
			int best = source;
			for (j = 0; j < TOTAL; j++)
				if (j != ME && cost[j][i] < cost[best][i])
					best = j;
			changed = 1;
			routing[i] = best;
		}
	}
	/* If rouitng table changed, notify neighbors */
	if (changed)
		broadcast();
}