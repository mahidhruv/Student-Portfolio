class DisjointSet {
    vector<int> rank, parent, size;
public:
    DisjointSet(int n) {
        rank.resize(n + 1, 0);
        parent.resize(n + 1);
        size.resize(n + 1);
        for(int i=0; i<=n; i++)
        {
            parent[i] = i;
            size[i] = 1;
        }
    }

    // Path compression -> find the parent of each node
    // by finding the parent of parent node, 
    // and then compress it.
    int findParent(int node)
    {
        if(node == parent[node])
        {
            return node;
        }
        return parent[node] = findParent(parent[node]);
    }

    bool find(int u, int v) {
        if(findParent(u) == findParent(v)) return true;
        return false;
    }

    void unionByRank(int u, int v) {
        // find the ultimate parent
        int ulp_u = findParent(u);
        int ulp_v = findParent(v);
        // means they are in same component
        if(ulp_u == ulp_v) return;
        // but if they are from different component then
        // we need to merge it into single one
        // Connect smaller rank to larger one always

        if(rank[ulp_u] < rank[ulp_v])
        {
            parent[ulp_u] = ulp_v;
        }
        else if(rank[ulp_v] < rank[ulp_u])
        {
            parent[ulp_v] = ulp_u;
        }
        // if dono k ultimate parent ki rank equal hai toh
        // kisi ko bhi attack ker sakte hai and rank will ++
        else
        {
            parent[ulp_v] = ulp_u;
            rank[ulp_u]++;
        }
    }

    void unionBySize(int u, int v) {
        // find the ultimate parent
        int ulp_u = findParent(u);
        int ulp_v = findParent(v);
        // means they are in same component
        if(ulp_u == ulp_v) return;
        // but if they are from different component then
        // we need to merge it into single one
        // Connect smaller size to larger one always

        // jo parent banta hai ultimately after merging
        // uska size badh jata hai.
        if(size[ulp_u] < size[ulp_v])
        {
            parent[ulp_u] = ulp_v;
            size[ulp_v] += size[ulp_u];
        }
        else
        {
            parent[ulp_v] = ulp_u;
            size[ulp_u] += size[ulp_v];
        }
    }
};
class Solution{
public:
    vector<vector<string>> accountsMerge(vector<vector<string>> accounts){
        int n = accounts.size();
        DisjointSet ds(n);
        unordered_map <string, int> mapMailNode;
        // Need to traverse on the emails
        // If the same email found then merge the nodes
        for(int i = 0; i<n; i++)
        {
            for(int j=1; j<accounts[i].size(); j++)
            {
                string mail = accounts[i][j];
                if(mapMailNode.find(mail) == mapMailNode.end())
                {
                    mapMailNode[mail] = i;
                }
                else
                {
                    ds.unionBySize(i, mapMailNode[mail]);
                }
            }
        }

        // Step 2 -> make the list of all the mails 
        // corresponding to the node

        vector<string> mergedMail[n];
        for(auto it: mapMailNode)
        {
            string mail = it.first;
            // It should be always the ultimate parent
            int node = ds.findParent(it.second);

            mergedMail[node].push_back(mail);
        }

        vector<vector<string>> ans;
        for(int i=0; i<n; i++)
        {
            if(mergedMail[i].size() == 0) continue;
            sort(mergedMail[i].begin(), mergedMail[i].end());
            vector<string> temp;
            temp.push_back(accounts[i][0]);
            for(auto it: mergedMail[i])
            {
                temp.push_back(it);
            }
            ans.push_back(temp);
        }
        return ans;
    }
};
