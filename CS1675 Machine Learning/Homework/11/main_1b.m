function main_1b

hist = [];

RL_init_model();
state = RL_reset_environment(model);
action = 0;

for i=1:10
    if state == 1
        action = 1;
    elseif state == 2
        action = 1;
    elseif state == 3
        action = 3;
    elseif state == 4
        action = 2;
    end
    [new_state reward] = RL_simulate_one_step(state, action, model);
    
    hist = [hist; state action reward];
    
    state = new_state;
end
    
    disp("   state action reward");
    disp(hist);

end

