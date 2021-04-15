function [test_y]  = DT_base_full(tr_x, tr_y, test_x, params)
%create a full tree model with no pruning
full_tree = fitctree(tr_x,tr_y,'splitcriterion','gdi','Prune','off');
%%% now we apply the model to test data and make a decision
test_y=predict(full_tree, test_x);

end