function [test_y]  = DT_base_simple(tr_x, tr_y, test_x, params)
%create a tree with only one split
tree = fitctree(tr_x,tr_y,'splitcriterion','gdi','MaxNumSplits',1);
%view(tree)
%%% now we apply the model to test data and make a decision
test_y=predict(tree, test_x);

end