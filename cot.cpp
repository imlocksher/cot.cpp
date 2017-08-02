//hld

#include<bits/stdc++.h>
#include<vector>
using namespace std;
#define root 0
#define N 10100
#define LN 14
vector<int> adj[N],cost[N],indexx[N];
int baseArray[N],ptr;
int chainNo,chainInd[N],chainHead[N],posInBase[N];
int depth[N],pa[LN][N],otherEnd[N],subsize[N];
int st[N*6],qt[6*N];
int LCA(int u, int v) {
	if(depth[u] < depth[v]) swap(u,v);
	int diff = depth[u] - depth[v];
	for(int i=0; i<LN; i++) if( (diff>>i)&1 ) u = pa[i][u];
	if(u == v) return u;
	for(int i=LN-1; i>=0; i--) if(pa[i][u] != pa[i][v]) {
		u = pa[i][u];
		v = pa[i][v];
	}
	return pa[0][u];
}

void HLD(int cur,int costs,int prev)
{

	if(chainHead[chainNo]==-1){
		chainHead[chainNo]=cur;
		}
	chainInd[cur]=chainNo;
	posInBase[cur]=ptr;
	baseArray[ptr++]=costs;
	
	int sc=-1;
	int ncost;
	
	for(int i=0;i<adj[cur].size();i++)
	{
		if(adj[cur][i]!=prev)
		{
			if(sc==-1||subsize[sc]<subsize[adj[cur][i]])
			{
				sc=adj[cur][i];
				ncost=cost[cur][i];
			}
		}
	}
	if(sc!=-1)
	{
		HLD(sc,ncost,cur);
	}
	
	for(int i=0;i<adj[cur].size();i++)
	{
		if(adj[cur][i]!=prev&&adj[cur][i]!=sc)
		{
			chainNo++;
			HLD(adj[cur][i],cost[cur][i],cur);
		}
	}
	
	


}
void dfs(int cur,int prev,int _depth=0)
{

	pa[0][cur]=prev;
	depth[cur]=_depth;
	subsize[cur] =1;
	for(int i=0;i<adj[cur].size();i++)
	{
		if(adj[cur][i]!=prev)
		{
			otherEnd[indexx[cur][i]]=adj[cur][i];
			dfs(adj[cur][i],cur,_depth+1);
			subsize[cur]+=subsize[adj[cur][i]];
		}
	}


}

void make_tree(int cur,int s,int e)
{
	if(s==e-1)
	{
		st[cur]=baseArray[s];
		return;
	}
	int c1 = (cur<<1),c2=c1|1,m=(e+s)>>1;
	make_tree(c1,s,m);
	make_tree(c2,m,e);
	st[cur]=st[c1]>st[c2]?st[c1]:st[c2];
}

void query_tree(int cur,int s,int e,int S,int E)
{
	if(s>=E||e<=S){
	qt[cur]=-1;
	return;
	}
	
	if(s>=S&&e<=E){
	
		qt[cur]=st[cur];
		return;
	
	}
	int c1=(cur<<1),c2=c1|1,m=(s+e)>>1;
	query_tree(c1,s,m,S,E);
	query_tree(c2,m,e,S,E);
	qt[cur]=qt[c1]>qt[c2]?qt[c1]:qt[c2];
}
int query_up(int u,int v)
{
	if(u==v)
	return 0;
	
	int uchain,vchain=chainInd[v],ans=-1;
	
	while(1)
	{
		uchain=chainInd[u];
		if(uchain==vchain)
		{
			if(u==v)break;
			query_tree(1,0,ptr,posInBase[v]+1,posInBase[u]+1);
			if(qt[1]>ans)
			ans=qt[1];
			break;
		
		
		}
		query_tree(1,0,ptr,posInBase[chainHead[uchain]],posInBase[u]+1);
		
		if(qt[1]>ans)
		ans=qt[1];
		
		u=chainHead[uchain];
		u=pa[0][u];
	
	}
return ans;


}

void query(int u,int v)
{
	int lca=LCA(u,v);
	int ans=query_up(u,lca);
	int temp = query_up(v,lca);
	if(temp>ans)ans=temp;
	printf("%d\n",ans);
}

int main()
{

	int t;
	scanf("%d",&t);
	while(t--)
	{
	
		ptr=0;
		int n;
		//cin>>n;
		scanf("%d",&n);
		//clearning 
		
		for(int i=0;i<n;i++)
		{
			adj[i].clear();
			cost[i].clear();
			indexx[i].clear();
			chainHead[i] = -1;
			for(int j=0;j<LN;j++)
				pa[j][i] =-1;

		}
		
		for(int i=1;i<n;i++)
		{
			int u,v,c;
			//cin>>u>>v>>c;
			scanf("%d%d%d",&u,&v,&c);
			u--;v--;
			adj[u].push_back(v);
			cost[u].push_back(c);
			indexx[u].push_back(i-1);
			adj[v].push_back(u);
			cost[v].push_back(c);
			indexx[v].push_back(i-1);
		
		}
		
		chainNo=0;
		dfs(root,-1);
		HLD(root,-1,-1);
		
		make_tree(1,0,ptr);
	
		for(int i=1;i<LN;i++)
		{
			for(int j=0;j<n;j++)
			{
				if(pa[i-1][j]!=-1)
				pa[i][j]=pa[i-1][pa[i-1][j]];
			}
		}
		while(1)
		{
		
			char s[100];
			scanf("%s",s);
			
			if(s[0]=='D')
			break;
			
			int a,b;
			scanf("%d%d",&a,&b);
			
			if(s[0]=='Q')
				query(a-1,b-1);
			else
				change(a-1,b);
			
		
		
		}
	
	
	}


return 0;
}
