# code-craft-2022

This is the mixture of several thoughts. 

## A Loser's words

I suppose that all of our group members could understand Chinese, so I would not bother myself using awkward English.

> 负优化 1
>
> 在考虑前 5% 时刻的分配问题时，我采取了以下贪心策略：
>
> 计算每个边缘节点在每个时刻，最多可以输出多少流量。取流量值最大的边缘节点以及对应的时刻，并分配掉该节点全部可分配的流量。重复以上操作，直到所有边缘节点的前 5% 时刻均得到分配。

> 负优化 2
>
> 在考虑后 95% 时刻的分配问题时，我采取了以下贪心策略：
>
> 对于每个时刻，对所有边缘节点按照历史最大流量值降序排序。在分配流量给每个客户节点时，按照排序的结果依次尝试分配。
>
> 不排除有实现得不够细腻的原因。在尝试分配的过程中，我并没有优先尝试填满每个边缘节点的历史最大值，而是直接暴力分配。这可能是导致成本爆炸的重要原因。
