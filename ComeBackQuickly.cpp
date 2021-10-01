#include<bits/stdc++.h>
using namespace std;
#define ll long long
#define vll vector<ll>
#define pll pair<ll,ll>
#define forA(i,a,n,x) for(ll i=a;i<n;i+=x)
#define forD(i,a,n,x) for(ll i=n-1;i>=a;i-=x)
#define ranit(it,n) for(auto it:n)
#define testCases ll t;cin>>t;while(t--)
#define mp make_pair
#define pb push_back
#define F first
#define S second
#define mod 1000000007
#define inf LLONG_MAX-1
#define init(a,x) memset(a,x,sizeof(a))

vector<pll> adj[2003];

void dijkstra(ll n , ll start)
{
	vll dist(n+1,inf);

	set<pll> s;
	s.insert({inf,start});

	while(!s.empty())
	{
		auto cur = s.begin();
		ll v = cur->S;
		s.erase(cur);

		ranit(it,adj[v])
		{
			ll prelen = 0;
			if(v==start)prelen=it.S;
			else prelen = dist[v]+it.S;

			if(prelen < dist[it.F])
			{
				s.erase({dist[it.F],it.F});
				dist[it.F] = prelen;
				s.insert({prelen, it.F});
			}

		}
	}

	//forA(i,1,n+1,1)cout<<dist[i]<<" "; cout<<"\n";
	ll ans = dist[start];
	if(ans==inf)ans=-1;
	cout<<ans<<"\n";
}

int main()
{
	ios_base::sync_with_stdio(false);	cin.tie(NULL);	cout.tie(NULL);
	ll n,m;
	cin>>n>>m;
	map<pll,ll> newmap;
	forA(i,0,m,1)
	{
		ll u,v,c; cin>>u>>v>>c;
		if(newmap.find({u,v})==newmap.end())newmap[{u,v}]=c;
		else newmap[{u,v}]=min(newmap[{u,v}],c);
	}

	ranit(it,newmap)
	{
		adj[it.F.F].pb({it.F.S,it.S});
	}

	forA(i,1,n+1,1)
	{
		dijkstra(n,i);
	}

	return 0;
}
