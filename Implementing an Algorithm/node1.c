#define INFINITY 999
#define TOTAL 4
#define ME 1

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

printdt1()
{
	int i, j;
	printf("The distance table of node %d:\n", ME);
	for (i = 0; i < TOTAL; i++) {
		for (j = 0; j < TOTAL; j++)
			printf("%4d", cost[i][j]);
		printf("\n");
	}
}

rtinit1()
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
	cost[0][0] = 1;
	cost[2][2] = 1;
	/* Broadcast */
	broadcast();
}

rtupdate1(rcvdpkt)
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

linkhandler1(linkid, newcost)
int linkid, newcost;
{
	/* Update distance table */
	int cost_diff = newcost - cost[linkid][linkid], i, j;
	for (i = 0; i < TOTAL; i++)
		cost[linkid][i] += cost_diff;
	cost[linkid][ME] += cost_diff;
	/* Rebuild routing table */
	for (i = 0; i < TOTAL; i++)
		/* Path through linkid need to be rebuild */
		if (i != ME && routing[i] == linkid) {
			/* Find the best new path */
			int best = linkid;
			for (j = 0; j < TOTAL; j++)
				if (j != ME && cost[j][i] < cost[best][i])
					best = j;
			routing[i] = best;
		}
	/* Broadcast */
	broadcast();
}
