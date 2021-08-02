set_long(
  "As you regain conciousness, you realize that you are no longer\n"
  +"where you previously were. $BOLD$REDRed$OFF and $BOLD$GREENgreen$OFF vines surround you,\n"
  +"thickly matted and forming walls around you. You can hear the\n"
  +"distant cries and wails of some unknown creatures. There is a\n"
  +"thick pink $BOLD$PINKfog$OFF hovering over the region, and visibility is very\n"
  +"limited. The last thing you remember was taking a big pull off\n"
  +"your sapho pipe. Is this real? Are you hallucinating?");

alias mazedefn lpc -&rs object md = clone_object("/players/bubbs/stuff/mazing_defn"); return strlen("$1") ? write(implode(map(md->get_maze("$1"), # +, "\n"), "")) : md->repl()

alias mazedefn lpc -&rs object md = clone_object("/players/bubbs/stuff/mazing_defn"); return strlen("$1") ? write(implode(map(md->get_maze("$1"), #'+, "\n"), "")) : md->repl()
















using dining_suite.Models;
using dining_suite.Views;
using System;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Threading.Tasks;
using Xamarin.Forms;

namespace dining_suite.ViewModels
{
    public class IngredientViewModel : BaseViewModel
    {
        private Models.Ingredient _selectedItem;

        public ObservableCollection<Models.Ingredient> Items { get; }
        public Command LoadItemsCommand { get; }
        public Command AddItemCommand { get; }
        public Command<Models.MenuItem> ItemTapped { get; }

        public IngredientViewModel()
        {
            Title = "Menu";
            Items = new ObservableCollection<Models.Ingredient>();
            LoadItemsCommand = new Command(async () => await ExecuteLoadItemsCommand());

            ItemTapped = new Command<Models.MenuItem>(this.OnItemSelected);

            AddItemCommand = new Command(OnAddItem);
        }

        async Task ExecuteLoadItemsCommand()
        {
            IsBusy = true;

            try
            {
                Items.Clear();
                var items = await DataStore.GetItemsAsync(true);
                foreach (var item in items)
                {
                    Items.Add(item);
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
            }
            finally
            {
                IsBusy = false;
            }
        }

        public void OnAppearing()
        {
            IsBusy = true;
            SelectedItem = null;
        }

        public Models.Ingredient SelectedItem
        {
            get => _selectedItem;
            set
            {
                SetProperty(ref _selectedItem, value);
                OnItemSelected(value);
            }
        }

        private async void OnAddItem(object obj)
        {
            await Shell.Current.GoToAsync(nameof(NewItemPage));
        }

        async void OnItemSelected(Models.Ingredient item)
        {
            if (item == null)
                return;

            // This will push the ItemDetailPage onto the navigation stack
            await Shell.Current.GoToAsync($"{nameof(ItemDetailPage)}?{nameof(MenuItemDetailViewModel.ItemId)}={item.Id}");
        }
    }
}